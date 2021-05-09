#ifndef POSTEFFECT_H
#define POSTEFFECT_H
#include "framebuffer.h"
#include "shaderprogram.h"
#include "material.h"
#include "camera.h"
#include <QOpenGLExtraFunctions>
#include "resourcemanager.h"
#include "vao.h"
#include <QDebug>
class PostEffect  {
public:
	PostEffect() { f = QOpenGLContext::currentContext()->extraFunctions(); };
	~PostEffect() {};
	virtual void update(Texture *mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer *gBuffer) {};
	void blit(Material* material, FrameBuffer* dest);
	int screenWidth = 0;
	int screenHeight = 0;
	int screenX = 0;
	int screenY = 0;
	void setViewport(int x, int y, int w, int h);
	VAO* screenVAO;
protected:
	QOpenGLExtraFunctions* f;
};

#endif