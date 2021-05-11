#ifndef SSR_H
#define SSR_H
#include "posteffect.h"
#include <random>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SSR : public PostEffect {
public:
	SSR();
	~SSR();
	void update(Texture* mainTex, FrameBuffer* dest, Camera* cam, FrameBuffer* gBuffer);
	void setViewport(int x, int y, int w, int h) override;
	Material ssrmat;
	Material blurmat;
	std::unique_ptr<FrameBuffer> ssrFBO;
	std::unique_ptr<FrameBuffer> blurFBO1, blurFBO2;
private:
	int SAMPLE_COUNT = 10;
	void generateRandomVec2();
	std::vector<glm::vec2> randomVec2;
};

#endif