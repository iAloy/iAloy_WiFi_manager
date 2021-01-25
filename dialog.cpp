#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::init()
{
	this->show();
}

void Dialog::on_pushButton_2_clicked()
{
	emit getPasswordConnectSignal(ui->passwordLineEdit->text());
	ui->passwordLineEdit->setText("");
	this->hide();
}
