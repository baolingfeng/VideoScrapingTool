#include "newappcopydialog.h"
#include <QMessageBox>

NewAppCopyDialog::NewAppCopyDialog(TplWindow *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	tpls = parent->tpls;

	for(int i=0; i<parent->tpls.size(); i++)
	{
		ui.comboBoxTpl->addItem(QString::fromStdString(parent->tpls[i].name));
	}

	connect(ui.comboBoxTpl,SIGNAL(currentIndexChanged(int)), this, SLOT(getAppList(int)));
	
	getAppList(0);
}

NewAppCopyDialog::~NewAppCopyDialog()
{

}

void NewAppCopyDialog::accept()
{
	if(getAppName().trimmed() == "")
	{
		QMessageBox::information(this, tr("Error"), tr("Application Name must be filled"));
		return;
	}

	QDialog::accept();
}

void NewAppCopyDialog::getAppList(int tplIndex)
{
	if(tplIndex<0) return;

	ui.comboBoxApp->clear();
	int sz = tpls[tplIndex].apps.size();
	for(int i=0; i<sz; i++)
	{
		ui.comboBoxApp->addItem(QString::fromStdString(tpls[tplIndex].apps[i].name));	
	}
	
}

QString NewAppCopyDialog::getAppName()
{
	return ui.editAppName->text();
}

int NewAppCopyDialog::getCopyTplIndex()
{
	return ui.comboBoxTpl->currentIndex();
}

int NewAppCopyDialog::getCopyAppIndex()
{
	return ui.comboBoxApp->currentIndex();
}
