#pragma once

#include <Windows.h>
#include <WinUser.h>
#include <QtWidgets/QWidget>
#include "ui_WowSay.h"
#include <qsystemtrayicon.h>
#include <qstring.h>
#include <qdebug.h>
#include <qobject.h>
#include <qkeysequence.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qclipboard.h>
#include <qdesktopwidget.h>
#include "QHotKey/QHotkey/qhotkey.h"
#include "QHotKey/QHotkey/qhotkey_p.h"
#include "QTray.h"

//#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define PRESENDTEXTKEY VK_RETURN
#define AFTERSENDTEXTKEY VK_RETURN
#define TARGETWINDOWNAME "’ΩΩ¢ ¿ΩÁ"
#define SENDKEYINTERVAL 100

class WowSay : public QWidget
{
    Q_OBJECT

public:
    WowSay(QWidget *parent = Q_NULLPTR);

public slots:
    void ShowHideWindow();
    void CheckText();
    void SendKeyControl();
    
private:
    Ui::WowSayClass ui;
    QHotkey* hkWake;
    QTimer* sendTimer;
    QClipboard* clipboard;
    QString oriClip;
    QTray* tray;
    int textEditFreeze = 0;
    int sendProc = 0;

    bool RegisterHotKey();
    bool SendText(QString text);
    bool SendPreKey();
    bool SendPasteKey();
    bool SendAfterKey();
};
