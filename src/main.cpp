#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef WIN32	// cout and qDebug do not work with vscode on Run without debugging (only Release configurations available)
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif
	QApplication a(argc, argv);

	MainWindow w;
	w.show();

	return a.exec();
}