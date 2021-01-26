#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QStringList>
#include <QListWidget>
#include <QProcess>
#include "dialog.h"

QT_BEGIN_NAMESPACE
using namespace std;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	QProcess *mProcess;
	QString WifiName;

private slots:
	void on_connectBtn_clicked();
	void on_wifiStatus_clicked();
	void Qprocess_start_tmp_slot();
	void Qprocess_read_tmp_slot();
	void Qprocess_end_tmp_slot(int);
	void getPasswordConnectSlot(QString password);

	void on_scanBtn_clicked();
//    void wifiStatusSlot();

signals:
	void getPasswordSignal();
//    void wifiStatusSignal();

private:
	Ui::MainWindow *ui;
	void addWiFiItems();
	QString nmctl;
	QStringList arguments;
	enum state
	{
		STATUS,
		ON,
		OFF,
		SEARCH,
		CONNECT,
		DISCONNECT,
		FORGET
	};
	int enum_status;
	int wifi_status;
	bool toggle_wifi;
};
#endif // MAINWINDOW_H
