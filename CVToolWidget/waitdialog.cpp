#include "waitdialog.h"

WaitDialog::WaitDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	QString temp = ui.label->text();
	//ui.label->setText("It will take a long time, please wait...");
}

WaitDialog::~WaitDialog()
{

}
