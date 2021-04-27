#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "framebuffer.h"
#include "shaderprogram.h"
#include "vao.h"
#include "commandqueue.h"
#include "camera.h"
#include <directionallight.h>
#include <ssao.h>
#include "lights/pointlight.h"

class DefRenderer : protected QOpenGLExtraFunctions {
public:
	DefRenderer(int x, int y, int width, int height);
	~DefRenderer();
	void setViewport(int x, int y, int width, int height);
	void clearQueue();
	void doGeometryPass(Camera* cam);
	CommandQueue& getQueue();
	FrameBuffer* getGBuffer();
	VAO* getScreenVAO();
	void addDirectionalLight(DirectionalLight* dirLight);
	void addPointLight(PointLight* dirLight);
	void doDirectionalLightPass(Camera* cam, SSAO* ssao);
	void doPointLightPass(Camera* cam);
	void doCombinePass(FrameBuffer* target);
	void setExposure(float exp);
private:
	CommandQueue deferredQueue;
	std::vector<DirectionalLight*> dirLights;
	std::vector<PointLight*> pointLights;
	std::unique_ptr<FrameBuffer> gBuffer;
	std::shared_ptr<ShaderProgram> gBufferShader;
	std::shared_ptr<ShaderProgram> dirLightShader;
	std::shared_ptr<ShaderProgram> pointLightShader;
	std::shared_ptr<ShaderProgram> combineShader;
	std::shared_ptr<Model> unitSphere;

	std::unique_ptr<VAO> screenVAO;
	//std::unique_ptr<SSAO> ssao;
	//unsigned int screenVAO, screenVBO;
	
	
	int m_width = 0;
	int m_height = 0;
	int m_xS = 0, m_yS = 0;
	float exposure = 0.5;

};