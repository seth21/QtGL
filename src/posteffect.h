#ifndef POSTEFFECT_H
#define POSTEFFECT_H
#include "framebuffer.h"
#include "shaderprogram.h"
class PostEffect {
public:
	PostEffect();
	~PostEffect();
	void blit(ShaderProgram* shader, FrameBuffer* src, FrameBuffer* dest);
	void update(unsigned int mainTex, FrameBuffer* dest, FrameBuffer* gBuffer);
private:
	void bindTextureAtUnit(unsigned int texHandle, unsigned int texUnit);
};

#endif