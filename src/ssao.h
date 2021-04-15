#ifndef SSAO_H
#define SSAO_H
#include <random>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "texture.h"
#include "resourcemanager.h"
#include "shaderprogram.h"
#include "framebuffer.h"
#include "vao.h"
#include <glm/mat4x4.hpp>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <glm/gtx/vector_angle.hpp>

class SSAO : protected QOpenGLExtraFunctions {
public:
	SSAO(int width, int height);
	~SSAO();
	void calculateSSAO(FrameBuffer *gBuffer, VAO *screenVAO, glm::mat4 projection, glm::mat4 view);
	void bindResult(int textureUnit);
	void screenSizeChanged(int width, int height);
	std::unique_ptr<FrameBuffer> ssaoBuffer;
	std::unique_ptr<FrameBuffer> blurBuffer;
private:
	int scrWidth = 0;
	int scrHeight = 0;
	std::vector<glm::vec3> ssaoKernel;
	std::vector<float> ssaoNoise;
	float lerp(float a, float b, float f);
	void generateKernelNoise();
	std::unique_ptr<Texture> noiseTexture;
	std::shared_ptr<ShaderProgram> ssaoShader;
	std::shared_ptr<ShaderProgram> blurShader;
};
#endif