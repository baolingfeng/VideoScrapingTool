#ifndef NEWAPPDIALOG_H
#define NEWAPPDIALOG_H

#include <QDialog>
#include "ui_NewApplicationDialog.h"

class NewAppDialog : public QDialog
{
	Q_OBJECT

public:
	NewAppDialog(QWidget *parent, int copy=0);
	~NewAppDialog();

	void accept();

	QString getAppName();
	int getRowNum();
	int getContentRow();
	int getUrlRow();
	void setRowNum(int);
	void setContentRow(int);
	void setUrlRow(int);

private:
	Ui::NewApplication ui;
};

#endif // NEWAPPDIALOG_H
