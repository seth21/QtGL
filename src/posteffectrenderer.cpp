#include "posteffectrenderer.h"

PostEffectRenderer::PostEffectRenderer(int xS, int yS, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	screenX = xS;
	screenY = yS;
	currBuffer = 0;
	
	for (int i = 0; i < 2; i++) {
		std::unique_ptr<FrameBuffer> fbo = std::make_unique<FrameBuffer>(width, height);
		fbo->registerColorAttachment(0, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_LINEAR, "post" + i);
		fbo->setRenderTargets(1, 0);
		fbo->setup();
		fbos.push_back(std::move(fbo));
	}
}

PostEffectRenderer::~PostEffectRenderer()
{
}

void PostEffectRenderer::render(Camera* cam, FrameBuffer* gBuffer, VAO *screenVAO)
{
	currBuffer = 0;
	for (int i = 0; i < effects.size(); i++) {
		Texture* mainTex = fbos[currBuffer]->getColorAttachments()[0]->tex.get();
		FrameBuffer* dest = nullptr;
		if (i != effects.size() - 1) {
			dest = fbos[currBuffer == 0 ? 1 : 0].get();
		}
		effects[i]->screenVAO = screenVAO;
		effects[i]->update(mainTex, dest, cam, gBuffer);
		currBuffer = 1 ? currBuffer == 0 : 0;
	}
}



void PostEffectRenderer::setViewport(int xS, int yS, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	screenX = xS;
	screenY = yS;
	for (int i = 0; i < 2; i++) {
		fbos[i]->setViewport(0, 0, width, height);
	}
	for (int i = 0; i < effects.size(); i++) {
		effects[i]->setViewport(xS, yS, width, height);
	}

}

int PostEffectRenderer::getActiveEffectsCount()
{
	return effects.size();
}

FrameBuffer* PostEffectRenderer::getDefaultFBO()
{
	return fbos[0].get();
}
