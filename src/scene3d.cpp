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
    //delete entity;

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

	//entity = new Entity();
	//entity->model = ResourceManager::getInstance().load<Model>("models/sponza.obj");
	//entity->scale = glm::vec3(0.05f);

	//postRenderer = std::make_unique<PostProcessingRenderer>(scrWidth, scrHeight);
	//ResourceConfig cubeConfig;
	//cubeConfig.flags.push_back("cube");
	//auto cubeTex = ResourceManager::getInstance().load<Texture>("textures/skybox/day/*right.jpg*left.jpg*top.jpg*bottom.jpg*front.jpg*back.jpg", cubeConfig);
	//auto skyShader = ResourceManager::getInstance().load<ShaderProgram>("skybox", cubeConfig);
	//skyRenderer = std::make_unique<SkyRenderer>(skyShader, cubeTex);
	//deferredRenderer = std::make_unique<DeferredRenderer>(scrWidth, scrHeight);
	
    mp_timer->start();
	world = std::make_unique<World>(0, 0, scrWidth, scrHeight);
}

void Scene3D::paintGL()
{
	//cam.update(deltaTime());
	world->update(deltaTime());
	world->render(deltaTime());
	//postRenderer->renderToScreen();
	//deferredRenderer->render(&cam, entity);
	
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
	//glViewport(xS, yS, ww, hh);

	//postRenderer->setViewport(scrX, scrY, scrWidth, scrHeight);
	//deferredRenderer->setViewport(scrX, scrY, scrWidth, scrHeight);
	world->setViewport(scrX, scrY, scrWidth, scrHeight);
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


