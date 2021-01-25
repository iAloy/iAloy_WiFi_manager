#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	Dialog *dia = new Dialog();
	connect(this, SIGNAL(getPasswordSignal()), dia, SLOT(init()));
	connect(dia, SIGNAL(getPasswordConnectSignal(QString)), this, SLOT(getPasswordConnectSlot(QString)));
	addWiFiItems();

	nmctl = "nmctl";
	mProcess = new QProcess(this);

	connect(mProcess, SIGNAL(started()), this, SLOT(Qprocess_start_tmp_slot()));
	connect(mProcess, SIGNAL(readyReadStandardOutput()),this,SLOT(Qprocess_read_tmp_slot()));
	connect(mProcess, SIGNAL(finished(int)), this, SLOT(Qprocess_end_tmp_slot(int)));
	connect(mProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
	{
		cout << "error enum val = " << error << endl;
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::addWiFiItems()
{

}

void MainWindow::on_connectBtn_clicked()
{
	enum_status = CONNECT;
	arguments << "connect" << ui->listWidget->currentItem()->text();
	mProcess->start(nmctl, arguments);
}

void MainWindow::on_wifiStatus_clicked()
{

}

void MainWindow::getPasswordConnectSlot(QString password)
{
	enum_status = CONNECT;
	arguments << "connect" << ui->listWidget->currentItem()->text() << password;
	mProcess->start(nmctl, arguments);
}

void MainWindow::Qprocess_start_tmp_slot()
{

}

void MainWindow::Qprocess_read_tmp_slot()
{
	switch (enum_status)
	{
		case SEARCH:
			{
				QJsonDocument d = QJsonDocument::fromJson(mProcess->readAllStandardOutput());
				QJsonArray itemArray = d.array();
				ui->listWidget->clear();
				foreach(const QJsonValue &wifi, itemArray)
				{
					ui->listWidget->addItem(wifi.toObject().value("SSID").toString());
				}
			}
		break;

		case CONNECT:
			{
				if(mProcess->readAllStandardOutput().toInt() == 1)
				{
					cout << "Connected" << endl;
				}
				else
				{
					emit getPasswordSignal();
				}
			}
		break;
	}
}

void MainWindow::Qprocess_end_tmp_slot(int)
{
	arguments.clear();
	mProcess->close();
}

void MainWindow::on_scanBtn_clicked()
{
	enum_status = SEARCH;
	arguments << "search";
	mProcess->start(nmctl, arguments);
}
