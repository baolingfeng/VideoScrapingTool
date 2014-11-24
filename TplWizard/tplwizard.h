#ifndef TPLWIZARD_H
#define TPLWIZARD_H

#include <QtWidgets/QWidget>
#include <QWizard>
#include "ui_tplwizard.h"

class TplWizard : public QWizard
{
	Q_OBJECT

public:
	TplWizard(QWidget *parent = 0);
	~TplWizard();

	void accept();

private:
	Ui::TplWizardClass ui;
};

#endif // TPLWIZARD_H
