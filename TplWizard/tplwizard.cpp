#include "tplwizard.h"
#include "FirstPage.h"
#include "SecondPage.h"
#include "ThirdPage.h"

TplWizard::TplWizard(QWidget *parent)
	: QWizard(parent)
{
	addPage(new FirstPage);
	//addPage(new SecondPage);
	addPage(new ThirdPage);
	
	this->setGeometry(0,0,1200,800);
}

TplWizard::~TplWizard()
{

}


void TplWizard::accept()
{
	QString apptype = field("apptype").toString();
	QString appname = field("appname").toString();
	int rownum = field("rownum").toInt();
	int content = field("content").toInt();
	int url = field("url").toInt();

	QDialog::accept();
}