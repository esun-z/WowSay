#pragma once
#include <qapplication.h>
#include <qobject.h>
#include <qsystemtrayicon.h>
#include <qmenu.h>
#include <qaction.h>

class QTray : QObject
{
	
	Q_OBJECT

public:
	QTray(QIcon icon, QObject* parent, QWidget* pWindow, QString toolTip);
	~QTray();
	QSystemTrayIcon* trayIcon;
public slots:
	void ActiveTray(QSystemTrayIcon::ActivationReason reason);
	void ShowWindow();
	void ExitWindow();
private:
	QMenu* menu;
	QAction* actionShow;
	QAction* actionExit;
	QWidget* w;
	void InitMenu(QWidget* parent);
};

