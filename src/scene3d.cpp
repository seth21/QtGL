#include "scene3d.h"
#include "shaderprogram.h"
#include <iostream>

#include <QDir>
#include <QApplication>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


Scene3D::Scene3D(QWidget *parent) : QGLWidget(parent)
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMouseTracking(true);
	//grabMouse();
}

Scene3D::~Scene3D()
{
    delete texture;
	delete model;
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
	// glViewport(0, 0, 800, 600);

	// build, compile and start our shader program
	shader.addFlag("ALBEDO");
	shader.addFlag("NORMAL");
	shader.init("default2");
	shader.start();
	shader.loadVector3f("lightPos", glm::vec3(40, 40, 0));
	shader.loadVector3f("ambientLight", glm::vec3(0.2, 0.2, 0.1));
	shader.loadVector3f("lightColor", glm::vec3(0.7, 0.6, 0.3));
	shader.loadMatrix4f("projMat", cam.getProjMatrix());
	model = new Model("crate.obj");

    texture = new Texture("crate.png");
	texNormal = new Texture("crateNormal.png");
	
	texture->bind(0);
	texNormal->bind(1);
	shader.loadInt("albedoMap", 0);
	shader.loadInt("normalMap", 1);

    mp_timer->start();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);  

}

void Scene3D::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cam.update(deltaTime());
    
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));
	shader.loadMatrix4f("viewMat", cam.getViewMatrix());
	shader.loadVector3f("viewPos", cam.position);
	//qDebug() << cam.up.x <<","<< cam.up.y << "," << cam.up.z;
	shader.loadMatrix4f("modelMat", Model);
	if (InputManager::getInstance().isPressed(FORWARD)) {
		//qDebug() << "FORWARD";
	}
	model->drawNow();
}

void Scene3D::resizeGL(int w, int h)
{
	//int side = qMin(w, h);
	//glViewport((w - side) / 2, (h - side) / 2, side, side);
	float aspectRatio = 4.0f / 3.0f;
	if (h * aspectRatio <= w) {
		int ww = h * aspectRatio;
		int hh = h;
		glViewport((w - ww) / 2, (h - hh) / 2, ww, hh);
	}
	else {
		int ww = w;
		int hh = (float)h / aspectRatio;
		glViewport((w - ww) / 2, (h - hh) / 2, ww, hh);
	}
	
	
}

float Scene3D::deltaTime(){
    return (mp_timer->remainingTime())*0.001f;
}

void Scene3D::closeEvent(QCloseEvent *event)
{
	makeCurrent();
	
    //glDeleteProgram(m_shaderProgram);
	doneCurrent();	
}


