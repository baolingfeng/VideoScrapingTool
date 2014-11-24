#ifndef CVTOOLWIDGET_H
#define CVTOOLWIDGET_H

#include <QtWidgets/QWidget>
#include "ui_cvtoolwidget.h"
#include "MyDefines.h"

class CVToolWidget : public QWidget
{
	Q_OBJECT

public:
	CVToolWidget(QWidget *parent = 0);
	~CVToolWidget();

public slots:
	void openImageFolder();
	void run();
	void openConfig();
	//void initConfigParam();
	void openTpl();

private:
	int numOfFrame;
	int minIndex;
	int maxIndex;
	int frameStart;
	int frameEnd;

	MyConfig config;

	Ui::CVToolWidgetClass ui;
};

#endif // CVTOOLWIDGET_H
