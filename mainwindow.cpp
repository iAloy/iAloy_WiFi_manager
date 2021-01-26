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
//	addWiFiItems();

	nmctl = "nmctl";
	mProcess = new QProcess(this);
	toggle_wifi = false;

	connect(mProcess, SIGNAL(started()), this, SLOT(Qprocess_start_tmp_slot()));
	connect(mProcess, SIGNAL(readyReadStandardOutput()),this,SLOT(Qprocess_read_tmp_slot()));
	connect(mProcess, SIGNAL(finished(int)), this, SLOT(Qprocess_end_tmp_slot(int)));
	connect(mProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
	{
		cout << "error enum val = " << error << endl;
	});
	enum_status = STATUS;
	arguments << "wifi";
	mProcess->start(nmctl, arguments);
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
	toggle_wifi = true;
	enum_status = STATUS;
	arguments << "wifi";
	mProcess->start(nmctl, arguments);
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
		case STATUS:
			{
				if(mProcess->readAllStandardOutput().toInt() == 1)
				{
					wifi_status = 1;
					ui->wifiStatus->setText("Turn Off");
				}
				else
				{
					wifi_status = 0;
					ui->wifiStatus->setText("Turn On");
				}
			}
		break;

		case SEARCH:
			{
				cout << "Searching..." << endl;
				QByteArray data = mProcess->readAllStandardOutput();
				QJsonDocument d = QJsonDocument::fromJson(data);
				QJsonArray itemArray = d.array();
				ui->listWidget->clear();
//                cout << data.toStdString() << endl;
				foreach(const QJsonValue &wifi, itemArray)
				{
					ui->listWidget->addItem(wifi.toObject().value("SSID").toString());
				}
				cout << "Searching...done" << endl;
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

		case ON:
			{
				if(mProcess->readAllStandardOutput().toInt() == 1)
				{
					wifi_status = 1;
					ui->wifiStatus->setText("Turn Off");
				}
				else
				{
					cout << "Something went wrong!!!" << endl;
				}
			}
		break;

		case OFF:
			{
				if(mProcess->readAllStandardOutput().toInt() == 1)
				{
					wifi_status = 0;
					ui->wifiStatus->setText("Turn On");
				}
				else
				{
					cout << "Something went wrong!!!" << endl;
				}
			}
		break;
	}
}

void MainWindow::Qprocess_end_tmp_slot(int)
{
	arguments.clear();
	mProcess->close();

	switch(enum_status)
	{
		case STATUS:
		{
			if(toggle_wifi)
			{
				if(wifi_status)
				{
					enum_status = OFF;
					arguments << "wifi" << "off";
					ui->wifiStatus->setText("Turning Off");
				}
				else
				{
					enum_status = ON;
					arguments << "wifi" << "on";
					ui->wifiStatus->setText("Turning On");
				}
				mProcess->start(nmctl, arguments);
			}
			else if(wifi_status)
			{
				enum_status = SEARCH;
				arguments << "search";
				mProcess->start(nmctl, arguments);
			}
		}
		break;

		case ON:
		{
			ui->scanBtn->setEnabled(true);
		}
		break;

		case OFF:
		{
			ui->listWidget->clear();
			ui->scanBtn->setEnabled(false);
		}
		break;

		case SEARCH:
		{
			ui->scanBtn->setEnabled(true);
		}
		break;
	}
}

void MainWindow::on_scanBtn_clicked()
{
	enum_status = SEARCH;
	arguments << "search";
	ui->scanBtn->setEnabled(false);
	mProcess->start(nmctl, arguments);
}
