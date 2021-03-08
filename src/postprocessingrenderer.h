#ifndef POSTPROCESSINGRENDERER_H
#define POSTPROCESSINGRENDERER_H

#include <QOpenGLExtraFunctions>
#include "shaderprogram.h"
#include "framebuffer.h"
#include <memory>
#include "resourcemanager.h"

class PostProcessingRenderer : protected QOpenGLExtraFunctions {
public:
	PostProcessingRenderer(int screenWidth, int screenHeight);
	~PostProcessingRenderer();
	void startPostRenderTarget();
	void renderToScreen();
	void setupScreenQuad();
	std::unique_ptr<FrameBuffer> mainFBO;
	std::shared_ptr<ShaderProgram> postShader;
	void setViewport(int x, int y, int width, int height);
	
private:
	unsigned int VAO = 0, VBO = 0;
	int width = 0;
	int height = 0;
	int xS = 0, yS = 0;
};
#endif // !POSTPROCESSINGRENDERER_H
