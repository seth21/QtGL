#ifndef SKYRENDERER_H
#define SKYRENDERER_H
#include <memory>
#include "shaderprogram.h"
#include "texture.h"
#include "camera.h"
#include <QOpenGLExtraFunctions>

class SkyRenderer : protected QOpenGLExtraFunctions {
public:
	SkyRenderer(std::shared_ptr<ShaderProgram> skyShader, std::shared_ptr<Texture> skyTexture);
	~SkyRenderer();
	void render(Camera* cam);
private:
	std::shared_ptr<ShaderProgram> skyShader;
	std::shared_ptr<Texture> skyTexture;
	unsigned int VAO = 0, VBO = 0;
	void generateCubeMesh();
	

};

#endif