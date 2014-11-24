#pragma once

#include <QWizardPage>
#include "ui_firstpage.h"

class FirstPage :
	public QWizardPage
{
	Q_OBJECT
public:
	FirstPage(QWidget *parent=0);
	~FirstPage(void);

public slots:
	void openTemplate();

public:
	Ui::UI_FirstPage ui;
};

