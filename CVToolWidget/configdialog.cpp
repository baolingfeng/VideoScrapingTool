#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.editFastCornerThre->setText("10");

}

ConfigDialog::~ConfigDialog()
{

}

void ConfigDialog::accept()
{
	fastCornerThreshold = ui.editFastCornerThre->text().toInt();

	QDialog::accept();
}