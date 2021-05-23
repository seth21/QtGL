#include "ssr.h"

SSR::SSR()
{
	//SSR
	ssrmat.shader = ResourceManager::getInstance().load<ShaderProgram>("ssr");
	generateRandomVec2();
	ShaderProgram* shader = ssrmat.shader.get();
	shader->start();
	for (int i = 0; i < SAMPLE_COUNT; i++) {
		shader->loadVector2f("RAND_SAMPLES[" + std::to_string(i) + "]", randomVec2[i]);
	}
	ssrFBO = std::make_unique<FrameBuffer>(screenWidth/2, screenHeight/2);
	ssrFBO->registerColorAttachment(0, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_LINEAR, "ssr");
	ssrFBO->registerColorAttachment(1, GL_UNSIGNED_BYTE, GL_RED, GL_RED, GL_LINEAR, "raydistance");
	ssrFBO->setRenderTargets(2, 0, 1);
	ssrFBO->setup();
	//Blur
	blurmat.shader = ResourceManager::getInstance().load<ShaderProgram>("weightedgblur");
	
	blurFBO1 = std::make_unique<FrameBuffer>(screenWidth / 2, screenHeight / 2);
	blurFBO1->registerColorAttachment(0, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_LINEAR, "blur");
	blurFBO1->setRenderTargets(1, 0);
	blurFBO1->setup();
	
	blurFBO2 = std::make_unique<FrameBuffer>(screenWidth / 2, screenHeight / 2);
	blurFBO2->registerColorAttachment(0, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_LINEAR, "blur");
	blurFBO2->setRenderTargets(1, 0);
	blurFBO2->setup();
	
	
}

SSR::~SSR()
{
}

void SSR::update(Texture* mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer* gBuffer)
{
	//Calculate SSR
	ssrmat.setTempTexture("mainTex", mainTex);
	ssrmat.setTempTexture("gPosition", gBuffer->getColorAttachment("position"));
	ssrmat.setTempTexture("gNormal", gBuffer->getColorAttachment("normal"));
	ssrmat.setTempTexture("gAlbedoSpec", gBuffer->getColorAttachment("albedospec"));
	ssrmat.setTempTexture("depthSampler", gBuffer->getDepthAttachment()->tex.get());
	ssrmat.setVec3("cameraPos", cam->position);
	ssrmat.setFloat("depthCheckBias", 0.0001f);
	ssrmat.setFloat("edgeCutOff", 0.1f);
	ssrmat.setFloat("maxBlurDist", 15);
	ssrmat.setMat4("projViewMat", cam->getPVmatrix());
	ssrmat.setMat4("invProjViewMat", glm::inverse(cam->getPVmatrix()));
	ssrmat.setVec2("camNearFar", glm::vec2(cam->nearPlane, cam->farPlane));
	ssrmat.setVec2("texelSize", glm::vec2(1.0f/ssrFBO->getWidth(), 1.0f/ssrFBO->getHeight()));
	blit(&ssrmat, ssrFBO.get());
	
	//Blur result
	//Horizontal
	blurmat.setVec2("texelSize", glm::vec2(1.0f / blurFBO1->getWidth(), 1.0f / blurFBO1->getHeight()));
	blurmat.setTempTexture("distTex", ssrFBO->getColorAttachment("raydistance"));
	
	blurmat.setVec2("direction", glm::vec2(1, 0));
	blurmat.setTempTexture("mainTexture", ssrFBO->getColorAttachment("ssr"));
	blurmat.setFloat("final", 0);
	blit(&blurmat, blurFBO1.get());
	//Vertical
	blurmat.setVec2("direction", glm::vec2(0, 1));
	blurmat.setTempTexture("mainTexture", blurFBO1->getColorAttachment("blur"));
	blurmat.setFloat("final", 1);
	blurmat.setTempTexture("sceneTex", mainTex);
	blit(&blurmat, dest);
}

void SSR::setViewport(int x, int y, int w, int h)
{
	ssrFBO->setViewport(x, y, w/2, h/2);
	blurFBO1->setViewport(x, y, w / 2, h / 2);
	blurFBO2->setViewport(x, y, w / 2, h / 2);
}

void SSR::generateRandomVec2()
{
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
	std::default_random_engine generator;
	//randomVec2.push_back(glm::vec2(0,0));
	//generate random samples around a circle
	while (randomVec2.size() < SAMPLE_COUNT)
	{
		glm::vec2 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0);
		sample = glm::normalize(sample);
		randomVec2.push_back(sample);
	}
}
