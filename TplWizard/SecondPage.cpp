#include "SecondPage.h"
#include <QRadioButton>
#include <QMessageBox>
#include <io.h>

SecondPage::SecondPage(QWidget *parent)
{
	ui.setupUi(this);

	_finddata_t fileDir;
	char* dir="C:/Users/LingFeng/Documents/Visual Studio 2010/Projects/CVTool2/data/*";
	long lfDir;

	if((lfDir=_findfirst(dir,&fileDir)) == -1)
	{
		 QMessageBox::information(this, tr("No file"), tr(dir));
	}
	else
	{
		do{
			if(strcmp(fileDir.name,".") != 0 && strcmp(fileDir.name,"..") != 0)
			{
				ui.comboBox->addItem(fileDir.name);
			}
        }while( _findnext( lfDir, &fileDir ) == 0 );
	}

	ui.comboBox->setCurrentIndex(-1);
	
	connect(ui.comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(appTypeSelected(QString)));

	registerField("apptype",ui.comboBox);
	registerField("appname",ui.appName);
	registerField("rownum",ui.spinBox_rownum);
	registerField("content",ui.spinBox_content);
	registerField("url",ui.spinBox_url);
}

void SecondPage::initializePage()
{

}

SecondPage::~SecondPage(void)
{
}

bool SecondPage::validatePage()
{
	QString apptype = ui.comboBox->currentText();
	QString appname = ui.appName->text();
	int rownum = ui.spinBox_rownum->value();

	if(apptype == "")
	{
		QMessageBox::information(this, tr("Error"), tr("Application Type must be selected"));
		return false;
	}

	if(appname == "")
	{
		QMessageBox::information(this, tr("Error"), tr("Application name can't be empty!"));
		return false;
	}

	if(rownum<=0)
	{
		QMessageBox::information(this, tr("Error"), tr("row num must be larger than 0"));
		return false;
	}

	return true;
}

void SecondPage::appTypeSelected(QString index)
{
	//QMessageBox::information(this,"selected",ui.comboBox->currentText());
	if(ui.comboBox->currentText() != "TaskBar")
	{
		ui.spinBox_content->setEnabled(true);
	}
	else
	{
		ui.spinBox_content->setEnabled(false);
		ui.spinBox_rownum->setValue(1);
	}
	
	if(ui.comboBox->currentText() == "Chrome" || ui.comboBox->currentText() == "Firefox" || 
		ui.comboBox->currentText() == "360Browser")
	{
		ui.spinBox_url->setEnabled(true);
	}
	else
	{
		ui.spinBox_url->setEnabled(false);
	}
}
