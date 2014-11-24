#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>
#include "ui_WaitDialog.h"

class WaitDialog : public QDialog
{
	Q_OBJECT

public:
	WaitDialog(QWidget *parent=0);
	~WaitDialog();

private:
	Ui::WaitDialog ui;
};

#endif // WAITDIALOG_H
