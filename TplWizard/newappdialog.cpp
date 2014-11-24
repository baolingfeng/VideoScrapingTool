#include "newappdialog.h"
#include <QMessageBox>

NewAppDialog::NewAppDialog(QWidget *parent, int type)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.editRowNum->setValidator(new QIntValidator(1,10,this));
	ui.editContentRow->setValidator(new QIntValidator(-1,10,this));
	ui.editUrlRow->setValidator(new QIntValidator(-1,10,this));

	if(type == 1) // copy from another application
	{
		ui.editContentRow->setReadOnly(true);
		ui.editRowNum->setReadOnly(true);
		ui.editUrlRow->setReadOnly(true);
	}
}

NewAppDialog::~NewAppDialog()
{

}

void NewAppDialog::accept()
{
	QString name = getAppName();
	if(name == "")
	{
		QMessageBox::information(this, tr("Error"), tr("Application Name must be filled"));
		return;
	}

	QDialog::accept();
}

QString NewAppDialog::getAppName()
{
	return ui.editAppName->text();
}

int NewAppDialog::getRowNum()
{
	QString temp = ui.editRowNum->text();
	if(temp.trimmed() == "")
	{
		return -1;
	}

	return temp.toInt();
}

int NewAppDialog::getContentRow()
{
	QString temp = ui.editContentRow->text();
	if(temp.trimmed() == "")
	{
		return -1;
	}

	return temp.toInt();
}

int NewAppDialog::getUrlRow()
{
	QString temp = ui.editUrlRow->text();
	if(temp.trimmed() == "")
	{
		return -1;
	}

	return temp.toInt();
}

void NewAppDialog::setRowNum(int rowNum)
{
	ui.editRowNum->setText(QString::number(rowNum));
}

void NewAppDialog::setContentRow(int contentRow)
{
	ui.editContentRow->setText(QString::number(contentRow));
}

void NewAppDialog::setUrlRow(int urlRow)
{
	ui.editUrlRow->setText(QString::number(urlRow));
}