#include "FirstPage.h"
#include <QFileDialog>

FirstPage::FirstPage(QWidget *parent)
{
	ui.setupUi(this);

	QButtonGroup* btnGroup = new QButtonGroup(this);

	btnGroup->addButton(ui.btn_create);
	btnGroup->addButton(ui.btn_modify);
	ui.btn_create->setChecked(true);

	connect(ui.btnOpen,SIGNAL(clicked()),this,SLOT(openTemplate()));

	registerField("create",ui.btn_create);
	registerField("modify",ui.btn_modify);
	registerField("folder",ui.editFolder);
}


FirstPage::~FirstPage(void)
{
}

void FirstPage::openTemplate()
{
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Open Template Folder")); 
	fileDialog->setFileMode(QFileDialog::Directory);

	if(fileDialog->exec() == QDialog::Accepted) 
	{ 
		ui.editFolder->setText(fileDialog->selectedFiles()[0]);
	}
}
