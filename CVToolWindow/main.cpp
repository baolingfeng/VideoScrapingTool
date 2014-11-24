#include "cvtoolwindow.h"
#include <QtWidgets/QApplication>
#include "ObjMan.h"



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	CVToolWindow w;
	w.show();
	return a.exec();
}
