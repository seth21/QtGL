#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <iostream>


MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent)
{
	// == WINDOW SETTINGS ==
	setWindowTitle("Basic Application");
	
	setMinimumSize(800, 450);
	
	mp_scene3D = new Scene3D(this);
	setCentralWidget(mp_scene3D);
	setMouseTracking(true);
	//grabMouse();
	// == MENU BAR ==
	QMenuBar * menuBar = new QMenuBar(this);
	setMenuBar(menuBar);

	// -- File menu --
	QMenu * menu = menuBar->addMenu("&File");

	// New
	QAction * action = new QAction("&New", this);
	connect(action, &QAction::triggered, this, &MainWindow::OnFileNew);
	menu->addAction(action);

	// Open
	action = new QAction("&Open", this);
	connect(action, &QAction::triggered, this, &MainWindow::OnFileOpen);
	menu->addAction(action);

	// Save
	action = new QAction("&Save", this);
	connect(action, &QAction::triggered, this, &MainWindow::OnFileSave);
	menu->addAction(action);

	// separator
	menu->addSeparator();

	// Exit
	action = new QAction("&Exit", this);
	connect(action, &QAction::triggered, this, &MainWindow::close);
	menu->addAction(action);

	// == STATUS BAR ==
	QStatusBar * statusBar = new QStatusBar(this);
	setStatusBar(statusBar);
}

// == PRIVATE SLOTS ==
void MainWindow::OnFileNew()
{
	statusBar()->showMessage("File -> New");
	qDebug() << "file new from debug";
	std::cerr << "file new from cout" << std::endl;
}

void MainWindow::OnFileOpen()
{
	statusBar()->showMessage("File -> Open");
}

void MainWindow::OnFileSave()
{
	statusBar()->showMessage("File -> Save");
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	if (event->isAutoRepeat())
	{
		return;
	}
	InputManager::getInstance().registerPress(event->key(), true);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
	if (event->isAutoRepeat())
	{
		return;
	}
	InputManager::getInstance().registerPress(event->key(), false);

}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
	InputManager::getInstance().registerMouse(event->pos().x(), event->pos().y());

}