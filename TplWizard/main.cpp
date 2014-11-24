#include "tplwizard.h"
#include <QtWidgets/QApplication>
#include "tplwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	TplWindow w;
	w.show();
	//TplWizard w;
	//w.show();
	return a.exec();
}
