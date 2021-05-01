#ifndef SCENE3D_H
#define SCENE3D_H

#include <QDebug>

#include <QGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTimer>
#include <QtMath>
#include <memory>

#include "ecs/world.h"

class Scene3D : public QGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT

public:
	Scene3D(QWidget *parent = 0);
	~Scene3D();

	QSize sizeHint() const override;
    void paintGL() override;
	
	int scrWidth = 0, scrHeight = 0, scrX = 0, scrY = 0;
	std::unique_ptr<World> world;
protected:
	void initializeGL() override;

	void resizeGL(int w, int h) override;
	
	void closeEvent(QCloseEvent *event) override;
protected slots:
	
private:
	QSurfaceFormat *mp_glSurface;
	
    QTimer *mp_timer;
    float deltaTime();

};

#endif	// SCENE3D_H
