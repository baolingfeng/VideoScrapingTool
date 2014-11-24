#pragma once
#include "qdialog.h"
#include "ui_setting.h"

class SettingDialog :
	public QDialog
{
	Q_OBJECT
public:
	SettingDialog(QWidget* parent=0);
	~SettingDialog(void);

private:
	Ui::Dialog ui;
};

