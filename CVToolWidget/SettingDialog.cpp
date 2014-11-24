#include "SettingDialog.h"


SettingDialog::SettingDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);

	ui.lineEdit->setText("../windefinitions/example/");
	ui.lineEdit_2->setText("D:/temp/output/");

}


SettingDialog::~SettingDialog(void)
{
}
