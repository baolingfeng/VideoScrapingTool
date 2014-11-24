#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "cvtoolwidget.h"
#include "winconfig.h"
#include <QtWidgets/QApplication>
#include "scvripper.h"

void createConsole()
{
    AllocConsole();
    SetConsoleTitle(L"Debug console");

    int hConHandle;
    long lStdHandle;

    FILE *fp;   // redirect unbuffered STDOUT to the console
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 ); 

    // redirect unbuffered STDIN to the console
    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "r" );
    *stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );  

    // redirect unbuffered STDERR to the console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stderr = *fp;
    setvbuf( stderr, NULL, _IONBF, 0 );
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	createConsole();

	ScvRipper s;
	s.show();
	//WinConfig w;
	//w.show();

	//CVToolWidget w;
	//w.show();
	return a.exec();
}
