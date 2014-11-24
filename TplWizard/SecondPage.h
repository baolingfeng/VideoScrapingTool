#pragma once
#include <QWizardPage>
#include "ui_secondpage.h"

class SecondPage :
	public QWizardPage
{
	Q_OBJECT
public:
	SecondPage(QWidget *parent=0);
	~SecondPage(void);

	bool validatePage ();
public slots:
	void appTypeSelected(QString);

protected:
     void initializePage();

public:
	Ui::UI_SecondPage ui;
};

