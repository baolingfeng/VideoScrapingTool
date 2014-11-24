#ifndef CVTOOLWINDOW_H
#define CVTOOLWINDOW_H

#include <QtWidgets/QWidget>
#include "ui_cvtoolwindow.h"

class CVToolWindow : public QWidget
{
	Q_OBJECT

public:
	CVToolWindow(QWidget *parent = 0);
	~CVToolWindow();

private:
	Ui::CVToolWindowClass ui;
};

#endif // CVTOOLWINDOW_H
