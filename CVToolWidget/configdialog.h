#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include "ui_Configuration.h"

class ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	ConfigDialog(QWidget *parent=0);
	~ConfigDialog();

	void accept();

public:
	int fastCornerThreshold;

private:
	Ui::ConfigDialog ui;
};

#endif // CONFIGDIALOG_H
