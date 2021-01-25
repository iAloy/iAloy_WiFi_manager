#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = nullptr);
	~Dialog();

private:
	Ui::Dialog *ui;

signals:
	void getPasswordConnectSignal(QString password);

private slots:
	void init();
	void on_pushButton_2_clicked();
};

#endif // DIALOG_H
