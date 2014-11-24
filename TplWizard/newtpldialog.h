#ifndef NEWTPLDIALOG_H
#define NEWTPLDIALOG_H

#include <QDialog>

#include "ui_NewTemplateDialog.h"

class NewTplDialog : public QDialog 
{
	Q_OBJECT

public:
	NewTplDialog(QWidget *parent=0);
	~NewTplDialog();

	QString getTplName();
private:
	Ui::NewTemplateDialog ui;
};

#endif // NEWTPLDIALOG_H
