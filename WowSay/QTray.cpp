#include "QTray.h"

QTray::QTray(QIcon icon, QObject* parent, QWidget* pWindow, QString toolTip) {
	w = pWindow;
	trayIcon = new QSystemTrayIcon(icon, parent);
	trayIcon->setToolTip(toolTip);
	InitMenu(w);
	trayIcon->setContextMenu(menu);
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(ActiveTray(QSystemTrayIcon::ActivationReason)));
	trayIcon->show();
	
}
QTray::~QTray() {
	trayIcon->hide();
	delete trayIcon;
	delete menu;
	delete actionShow;
	delete actionExit;
}

void QTray::InitMenu(QWidget* parent) {
	menu = new QMenu(parent);
	actionShow = new QAction(menu);
	actionExit = new QAction(menu);
	actionShow->setText("Show");
	actionExit->setText("Exit");
	menu->addAction(actionShow);
	menu->addAction(actionExit);
	connect(actionShow, SIGNAL(triggered()), this, SLOT(ShowWindow()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(ExitWindow()));
}

void QTray::ActiveTray(QSystemTrayIcon::ActivationReason reason) {
	switch (reason)
	{
	case QSystemTrayIcon::Unknown:
		menu->show();
		break;
	case QSystemTrayIcon::Context:
		menu->show();
		break;
	case QSystemTrayIcon::DoubleClick:
		ShowWindow();
		break;
	case QSystemTrayIcon::Trigger:
		ShowWindow();
		break;
	case QSystemTrayIcon::MiddleClick:
		ExitWindow();
		break;
	default:
		break;
	}
}

void QTray::ShowWindow() {
	w->show();
	w->raise();
	w->activateWindow();
	w->raise();
	w->activateWindow();
}

void QTray::ExitWindow() {
	w->close();
}