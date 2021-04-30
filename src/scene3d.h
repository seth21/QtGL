#ifndef SCENE3D_H
#define SCENE3D_H
#ifndef GLDEBUG
#define GLDEBUG
#endif
#ifndef NUMBERS
#define NUMBERS
#define N_AXES (3)
#define N_ELEMENTS (2)
#define N_ELEMENTITEMS (3)
#define N_VERTICES (4)
#define N_COLORARGS (3)
#define N_TEXAXES (2)
#endif
#include <QDebug>

#include <QGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTimer>
#include <QtMath>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <memory>
#include "shaderprogram.h"
#include "texture.h"
#include "model.h"
#include "inputmgr.h"
#include "action.h"
#include "camera.h"
#include "resourcemanager.h"
#include "entity.h"
#include "postprocessingrenderer.h"
#include "skyrenderer.h"
#include "ecs/world.h"

class Scene3D : public QGLWidget, protected QOpenGLExtraFunctions
{
	Q_OBJECT

public:
	Scene3D(QWidget *parent = 0);
	~Scene3D();

	QSize sizeHint() const override;
    void paintGL() override;
	//Entity* entity;
	//std::unique_ptr<DeferredRenderer> deferredRenderer;
	//std::unique_ptr<PostProcessingRenderer> postRenderer;
	//std::unique_ptr<SkyRenderer> skyRenderer;
	
	//std::shared_ptr<Texture> skyTexture;
	int scrWidth = 0, scrHeight = 0, scrX = 0, scrY = 0;
	std::unique_ptr<World> world;
protected:
	void initializeGL() override;

	void resizeGL(int w, int h) override;
	
	void closeEvent(QCloseEvent *event) override;
protected slots:
	
private:
	QSurfaceFormat *mp_glSurface;
	
    //std::shared_ptr<ShaderProgram> shader;
    QTimer *mp_timer;
    float deltaTime();

	//Camera cam;
};

#endif	// SCENE3D_H
