#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef WIN32	// cout and qDebug do not work with vscode on Run without debugging (only Release configurations available)
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);
	//freopen("CONOUT$", "w", stderr);
#endif
	QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	app.setApplicationName("cube");
	app.setApplicationVersion("0.1");

	MainWindow w;
	w.show();

	return app.exec();
}