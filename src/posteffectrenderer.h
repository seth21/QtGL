#ifndef POSTEFFECTRENDERER_H
#define POSTEFFECTRENDERER_H
#include "posteffect.h"
#include <vector>
#include <memory>
#include "texture.h"
#include "framebuffer.h"
#include "vao.h"
#include "camera.h"
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

class PostEffectRenderer : protected QOpenGLFunctions {
public:
	PostEffectRenderer(int xS, int yS, int width, int height);
	~PostEffectRenderer();
	template<typename T>
	T* addEffect();
	void render(Camera *cam, FrameBuffer *gBuffer, VAO* screenVAO);
	void setViewport(int xS, int yS, int width, int height);
	int getActiveEffectsCount();
	FrameBuffer* getDefaultFBO();

private:
	unsigned int currBuffer;
	std::vector<std::unique_ptr<FrameBuffer>> fbos;
	std::vector<std::unique_ptr<PostEffect>> effects;
	int screenWidth;
	int screenHeight;
	int screenX;
	int screenY;
	
};
#endif

template<typename T>
inline T* PostEffectRenderer::addEffect()
{
	static_assert(std::is_base_of<PostEffect, T>::value, "ERROR - Object added to PostEffectRenderer must inherit from PostEffect!");
	auto effect = std::make_unique<T>();
	effects.push_back(std::move(effect));
	return dynamic_cast<T*>(effects[effects.size() - 1].get());
}