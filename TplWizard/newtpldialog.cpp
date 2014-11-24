#include "newtpldialog.h"

NewTplDialog::NewTplDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

NewTplDialog::~NewTplDialog()
{

}

QString NewTplDialog::getTplName()
{
	return ui.lineEdit->text();
}