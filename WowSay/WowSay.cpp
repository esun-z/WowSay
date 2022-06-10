#include "WowSay.h"

WowSay::WowSay(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    if (RegisterHotKey()) {
        qDebug() << "Registered.";
        //connect(hkWake, &QHotkey::activated, this, SLOT(Wake()));
        connect(hkWake, &QHotkey::activated, this, [&]() {
            qDebug() << "Hotkey Activated.";
            ShowHideWindow();
        });
        
    }
    else {
        qDebug() << "* Failed to register hotkey.";
    }
    
    sendTimer = new QTimer(this);
    sendTimer->setSingleShot(true);
    connect(sendTimer, SIGNAL(timeout()), this, SLOT(SendKeyControl()));
    connect(ui.textEdit, SIGNAL(textChanged()), this, SLOT(CheckText()));

    tray = new QTray(QIcon(":/WowSay/res/icon.ico"), this, this, "WowSay");

    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    setWindowOpacity(0.75);

    QDesktopWidget* desktop = QApplication::desktop();
    move(0, (desktop->height() - this->height()) / 3 * 2);
}

bool WowSay::RegisterHotKey() {
    hkWake = new QHotkey(QKeySequence(Qt::CTRL + Qt::Key_Enter), true, this);
    return hkWake->isRegistered();
}

void WowSay::ShowHideWindow() {
    if (isActiveWindow()) {
        qDebug() << "Hide Window.";
        hide();
    }
    else {
        qDebug() << "Show Window.";
        show();
        raise();
        activateWindow();
        raise();
        activateWindow();
    }
}

void WowSay::CheckText() {
    if (textEditFreeze > 0) {
        textEditFreeze--;
        return;
    }
    QString s = ui.textEdit->toPlainText();
    if (s.endsWith("\n")) {
        qDebug() << "Send Text.";
        hide();
        textEditFreeze++;
        ui.textEdit->clear();
        s = s.left(s.length() - 1);
        if (!s.isEmpty()) {
            if (!SendText(s)) {
                tray->trayIcon->showMessage("WowSay Error", QString::fromStdString("* Failed to find window:\n") + QString::fromLocal8Bit(TARGETWINDOWNAME), QIcon(":/WowSay/res/icon.ico"));
            }
        }
    }
}

bool WowSay::SendText(QString text) {
    
    //setWindowFlag(Qt::WindowStaysOnTopHint, false);
    HWND hTarget = FindWindowA(NULL, TARGETWINDOWNAME);
    if (hTarget == NULL) {
        qDebug() << "* Failed to find target window.";
        return false;
    }
    if (SetForegroundWindow(hTarget) == 0) {
        qDebug() << "* Failed to bring target window to foreground.";
        return false;
    }

    clipboard = QApplication::clipboard();
    
    oriClip = clipboard->text();
    clipboard->setText(text);

    sendTimer->start(SENDKEYINTERVAL);

    return true;
}

void WowSay::SendKeyControl() {
    switch (sendProc) {
    case 0:
        if (SendPreKey()) {
            qDebug() << "PreKey Sent.";
            sendProc++;
            sendTimer->start(SENDKEYINTERVAL);
        }
        else {
            qDebug() << "* SendKey stopped on: PreKey";
        }
        break;
    case 1:
        if (SendPasteKey()) {
            qDebug() << "PasteKey Sent.";
            sendProc++;
            sendTimer->start(SENDKEYINTERVAL);
        }
        else {
            qDebug() << "* SendKey stopped on: PasteKey";
        }
        break;
    case 2:
        if (SendAfterKey()) {
            qDebug() << "AfterKey Sent.";
            sendProc = 0;
        }
        else {
            qDebug() << "* SendKey stopped on: AfterKey";
        }
        clipboard = QApplication::clipboard();
        clipboard->setText(oriClip);
        
        break;
    default:
        qDebug() << "* Unexpedted control code.";
        break;
    }
}

bool WowSay::SendPreKey() {
    bool suc = true;
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = PRESENDTEXTKEY;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = PRESENDTEXTKEY;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        qDebug() << "* Failed to send input to target window.\n";
        qDebug() << HRESULT_FROM_WIN32(GetLastError());
        suc = false;
        //OutputString(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
    
    return suc;
}

bool WowSay::SendPasteKey() {
    bool suc = true;
    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'V';

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'V';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        qDebug() << "* Failed to send input to target window.\n";
        qDebug() << HRESULT_FROM_WIN32(GetLastError());
        suc = false;
        //OutputString(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
    
    return suc;
}

bool WowSay::SendAfterKey() {
    bool suc = true;
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = AFTERSENDTEXTKEY;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = AFTERSENDTEXTKEY;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        qDebug() << "* Failed to send input to target window.\n";
        qDebug() << HRESULT_FROM_WIN32(GetLastError());
        suc = false;
        //OutputString(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }

    return suc;
}




