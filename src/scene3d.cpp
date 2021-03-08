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
    delete entity;

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

	// build, compile and start our shader program
	ResourceConfig shaderConfig;
	shaderConfig.flags.push_back("ALBEDO");
	shader = ResourceManager::getInstance().load<ShaderProgram>("default2", shaderConfig);
	//shader.addFlag("ALBEDO");
	//shader.addFlag("NORMAL");
	//shader.init("default2");
	shader->start();
	shader->loadVector3f("lightPos", glm::vec3(0, 90, 20));
	shader->loadVector3f("ambientLight", glm::vec3(0.2, 0.2, 0.1));
	shader->loadVector3f("lightColor", glm::vec3(0.7, 0.6, 0.3));
	shader->loadMatrix4f("projMat", cam.getProjMatrix());
	shader->loadInt("albedoMap", 0);
	//shader.loadInt("normalMap", 1);

	entity = new Entity();
	entity->model = ResourceManager::getInstance().load<Model>("models/sponza.obj");
	entity->scale = glm::vec3(0.05f);

	postRenderer = std::make_unique<PostProcessingRenderer>(scrWidth, scrHeight);
	ResourceConfig cubeConfig;
	cubeConfig.flags.push_back("cube");
	auto cubeTex = ResourceManager::getInstance().load<Texture>("textures/skybox/day/*right.jpg*left.jpg*top.jpg*bottom.jpg*front.jpg*back.jpg", cubeConfig);
	auto skyShader = ResourceManager::getInstance().load<ShaderProgram>("skybox", cubeConfig);
	skyRenderer = std::make_unique<SkyRenderer>(skyShader, cubeTex);

	
    mp_timer->start();

}

void Scene3D::paintGL()
{

	postRenderer->startPostRenderTarget();
	cam.update(deltaTime());
	skyRenderer->render(&cam);
	shader->start();
	shader->loadMatrix4f("viewMat", cam.getViewMatrix());
	shader->loadVector3f("viewPos", cam.position);
	entity->drawNow(shader.get());
	
	postRenderer->renderToScreen();
	
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

	postRenderer->setViewport(scrX, scrY, scrWidth, scrHeight);
	
	
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


