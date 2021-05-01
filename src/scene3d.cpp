#include "scene3d.h"
#include <iostream>

#include <QDir>
#include <QApplication>




Scene3D::Scene3D(QWidget *parent) : QGLWidget(parent)
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMouseTracking(true);
	//grabMouse();
}

Scene3D::~Scene3D()
{

}

QSize Scene3D::sizeHint() const
{
	return QSize(400, 400);
}

void Scene3D::initializeGL()
{
    mp_timer = new QTimer(this);
    mp_timer->setSingleShot(false);
    mp_timer->setInterval(1000.0/60.0);
    connect(mp_timer, &QTimer::timeout, this, &Scene3D::paintGL);
    connect(mp_timer, SIGNAL(timeout()), this, SLOT(update()));

	initializeOpenGLFunctions();
	qDebug() << reinterpret_cast<const char *>(glGetString(GL_RENDERER));

    mp_timer->start();
	world = std::make_unique<World>(0, 0, scrWidth, scrHeight);
}

void Scene3D::paintGL()
{
	world->update(deltaTime());
	world->render(deltaTime());
	
}

void Scene3D::resizeGL(int w, int h)
{
	float aspectRatio = 4.0f / 3.0f;
	if (h * aspectRatio <= w) {
		scrWidth = h * aspectRatio;
		scrHeight = h;
	}
	else {
		scrWidth = w;
		scrHeight = (float)w / aspectRatio;
	}
	scrX = (w - scrWidth) / 2;
	scrY = (h - scrHeight) / 2;

	world->setViewport(scrX, scrY, scrWidth, scrHeight);
}

float Scene3D::deltaTime(){
    return (mp_timer->remainingTime())*0.001f;
}

void Scene3D::closeEvent(QCloseEvent *event)
{
	makeCurrent();
	doneCurrent();	
}


