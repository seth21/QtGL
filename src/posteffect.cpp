#include "posteffect.h"

void PostEffect::blit(Material* material, FrameBuffer* dest)
{
	if (!dest) {
		f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
		f->glViewport(screenX, screenY, screenWidth, screenHeight);
		
	}
	else {
		dest->bind();
		f->glViewport(0, 0, dest->getWidth(), dest->getHeight());
	}
	//do blit stuff
	ShaderProgram* shader = material->shader.get();
	shader->start();
	if (!shader) return;
	int counter = 0;
	for (auto tex : material->getTextures()) {
		shader->loadInt(tex.first, counter);
		tex.second->bind(counter);
		counter++;
	}
	for (auto tex : material->getTempTextures()) {
		shader->loadInt(tex.first, counter);
		tex.second->bind(counter);
		counter++;
	}
	for (auto f : material->getFloats()) {
		shader->loadFloat(f.first, f.second);
	}
	for (auto v3 : material->getVec3s()) {
		shader->loadVector3f(v3.first, v3.second);
	}
	
	screenVAO->bind();
	f->glDrawArrays(GL_TRIANGLES, 0, 6);

	material->getTempTextures().clear();
}

void PostEffect::setViewport(int x, int y, int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	screenX = x;
	screenY = y;
}




