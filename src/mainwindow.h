/*
 * How to create an application with Qt and C++
 *
 * example code of blog post: http://blog.davidecoppola.com/2016/10/how-to-create-an-application-with-qt-and-cpp/
 *
*/

#pragma once

#include <QMainWindow>
#include "scene3d.h"
#include <QKeyEvent>
#include "inputmgr.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = nullptr);
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
private slots:
	void OnFileNew();
	void OnFileOpen();
	void OnFileSave();
	
private:
	Scene3D *mp_scene3D;
};