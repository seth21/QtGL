#include "deferredrenderer.h"

float quadVerts[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

DeferredRenderer::DeferredRenderer(int width, int height)
{
	initializeOpenGLFunctions();
	screenVAO = std::make_unique<VAO>();
	int vaoPos = screenVAO->createAttribute(VertexAttrib::POSITION, 0, 2, VAO::screenQuadPos, 12);
	int vaoTex = screenVAO->createAttribute(VertexAttrib::TEXCOORDS, 1, 2, VAO::screenQuadUV, 12);
	screenVAO->upload();
	//setupScreenQuad();
	gBuffer = std::make_unique<FrameBuffer>(width, height);
	//POSITIONS
	gBuffer->registerColorAttachment(0, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_NEAREST, "position");
	//NORMALS
	gBuffer->registerColorAttachment(1, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_NEAREST, "normal");
	//COLOR+SPEC
	gBuffer->registerColorAttachment(2, GL_UNSIGNED_BYTE, GL_RGBA, GL_RGBA, GL_NEAREST, "albedospec");
	//LIGHT
	gBuffer->registerColorAttachment(3, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_LINEAR, "light");
	//DEPTH
	gBuffer->registerDepthAttachment(GL_UNSIGNED_BYTE, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_NEAREST, "depth");
	gBuffer->setRenderTargets(4, 0, 1, 2, 3);
	gBuffer->setup();

	ResourceConfig shaderConfig;
	shaderConfig.flags.push_back("ALBEDO");
	shaderConfig.flags.push_back("BUMP");
	gBufferShader = ResourceManager::getInstance().load<ShaderProgram>("gbuffer", shaderConfig);
	dirLightShader = ResourceManager::getInstance().load<ShaderProgram>("gdirlight");
	pointLightShader = ResourceManager::getInstance().load<ShaderProgram>("gpointlight");
	combineShader = ResourceManager::getInstance().load<ShaderProgram>("gfinal");
	dirLightDepthShader = ResourceManager::getInstance().load<ShaderProgram>("dirlightdepth");
	debugRenderer = std::make_unique<DebugRenderer>();
	pointLight = std::make_unique<Entity>();
	pointLight->model = ResourceManager::getInstance().load<Model>("models/uniticosphere.obj");
	pointLight->position = glm::vec3(0, 4, 0);
	pointLight->scale = glm::vec3(6);
	dirLight = std::make_unique<DirectionalLight>();

	ssao = std::make_unique<SSAO>(width, height);
	postRenderer = std::make_unique<PostEffectRenderer>(0, 0, width, height);
	FXAA* fxaa = postRenderer->addEffect<FXAA>();
	ColorCorrection *correction = postRenderer->addEffect<ColorCorrection>();
	correction->exposure = exposure;
	
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::render(Camera* cam, Entity* entity)
{
	//LIGHT SHADOW PASS-------
	doDirectionalLightDepthPass(cam, entity);
	//GEOMETRY PASS-----------
	gBuffer->bind();
	gBuffer->setRenderTargets(3, 0, 1, 2);
	doGeometryPass(cam, entity);
	//SSAO--------------------
	doSSAO(cam);
	//LIGHTS------------------
	gBuffer->bind();
	gBuffer->setRenderTargets(1, 3); //Render to the light target only
	glClear(GL_COLOR_BUFFER_BIT);
	doDirectionalLightPass(cam, entity);
	doPointLightPass(cam, entity);
	
	doCombinePass(cam, entity);
	doPostProcessing(cam);
}

void DeferredRenderer::setViewport(int x, int y, int width, int height)
{
	gBuffer->setViewport(x, y, width, height);
	ssao->screenSizeChanged(width, height);
	postRenderer->setViewport(x, y, width, height);
	this->width = width;
	this->height = height;
	this->xS = x;
	this->yS = y;
}

DebugRenderer* DeferredRenderer::getDebugRenderer()
{
	return debugRenderer.get();
}

/*void DeferredRenderer::setupScreenQuad() {

	// Create buffers/arrays
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);

	glBindVertexArray(screenVAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(0));
	glEnableVertexAttribArray(0);
	// Vertex Texture Coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}*/

void DeferredRenderer::doGeometryPass(Camera* cam, Entity* entity)
{
	//GEOMETRY PASS
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak into g-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, gBuffer->getWidth(), gBuffer->getHeight());
	gBufferShader->start();
	gBufferShader->loadMatrix4f("viewMat", cam->getViewMatrix());
	gBufferShader->loadMatrix4f("projMat", cam->getProjMatrix());
	gBufferShader->loadVector3f("viewPos", cam->position);
	/*
	for(Object obj : Objects)
	{
		ConfigureShaderTransformsAndUniforms();
		obj.Draw();
	}  */
	//DRAW ENTITIES
	entity->drawNow(gBufferShader.get(), cam, true);

}

void DeferredRenderer::doDirectionalLightPass(Camera* cam, Entity* entity)
{
	//DIRECTIONAL LIGHT
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
	dirLightShader->start();
	//Bind GBuffer Textures
	dirLightShader->loadInt("gPosition", 0);
	dirLightShader->loadInt("gNormal", 1);
	dirLightShader->loadInt("gAlbedoSpec", 2);
	gBuffer->bindColorAttachment(0);
	gBuffer->bindColorAttachment(1);
	gBuffer->bindColorAttachment(2);
	//Lighting Uniforms
	dirLightShader->loadVector3f("lightDir", dirLight->getDirection());
	dirLightShader->loadVector3f("ambientLight", glm::vec3(0.11, 0.11, 0.06));
	dirLightShader->loadVector3f("lightColor", glm::vec3(0.9, 0.8, 0.3));
	dirLightShader->loadVector3f("viewPos", cam->position);
	dirLightShader->loadInt("shadowMap", 3);
	dirLight->getShadowMap()->bindDepthAttachment(3);
	dirLightShader->loadMatrix4f("shadowMapSpaceMatrix", dirLight->getToShadowMapSpaceMatrix());
	dirLightShader->loadFloat("shadowDistance", 250);
	dirLightShader->loadFloat("transitionDistance", 220);
	//ssao
	dirLightShader->loadInt("ssaoTexture", 4);
	ssao->bindResult(4);
	glViewport(0, 0, gBuffer->getWidth(), gBuffer->getHeight());
	//Render Directional Light as a screen-sized Quad to render attachment 3
	//glBindVertexArray(screenVAO);
	screenVAO->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DeferredRenderer::doDirectionalLightDepthPass(Camera* cam, Entity* entity)
{
	dirLight->getShadowMap()->bind();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glClear(GL_DEPTH_BUFFER_BIT);
	dirLight->updateMatrices(cam);
	dirLightDepthShader->start();
	dirLightDepthShader->loadMatrix4f("projMat", dirLight->getProjectionMatrix());
	dirLightDepthShader->loadMatrix4f("lightViewMat", dirLight->getLightViewMatrix());
	glViewport(0, 0, dirLight->getShadowMapSize(), dirLight->getShadowMapSize());
	entity->drawNow(dirLightDepthShader.get(), cam, false);
}

void DeferredRenderer::doPointLightPass(Camera* cam, Entity* entity)
{
	glm::vec3 lightColor = glm::vec3(12, 0, 2);
	float constant = 1;
	float linear = 0.7f;
	float quadratic = 1.8f;
	//POINT LIGHTS
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glCullFace(GL_FRONT);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	
	pointLightShader->start();
	pointLightShader->loadMatrix4f("pv", cam->getPVmatrix());
	pointLightShader->loadInt("gPosition", 0);
	pointLightShader->loadInt("gNormal", 1);
	pointLightShader->loadInt("gAlbedoSpec", 2);
	pointLightShader->loadFloat("light.constant", constant);
	pointLightShader->loadFloat("light.linear", linear);
	pointLightShader->loadFloat("light.quadratic", quadratic);
	pointLightShader->loadVector3f("light.color", lightColor);
	pointLightShader->loadVector3f("light.position", pointLight->position);
	pointLightShader->loadVector2f("gBufferSize", glm::vec2(gBuffer->getWidth(), gBuffer->getHeight()));
	pointLightShader->loadVector3f("viewPos", cam->position);
	float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
	float radius =
		(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (exposure * 256.0 / 3.0) * lightMax)))
		/ (2 * quadratic);
	pointLightShader->loadFloat("light.maxRadius", radius);
	pointLight->scale = glm::vec3(radius * 1.03f); //extend the radius slightly so we dont see the sphere's triangles
	gBuffer->bindColorAttachment(0);
	gBuffer->bindColorAttachment(1);
	gBuffer->bindColorAttachment(2);
	//glDepthMask(GL_FALSE);
	//glDepthFunc(GL_GEQUAL);
	//glCullFace(GL_FRONT);
	pointLight->drawNow(pointLightShader.get(), cam, false);
	glDisable(GL_BLEND);
}

void DeferredRenderer::doCombinePass(Camera* cam, Entity* entity)
{
	//IS THERE POST RENDERING OR SHOW DIRECTLY TO SCREEN?
	if (postRenderer->getActiveEffectsCount() == 0) {
		gBuffer->unbind();
		glViewport(xS, yS, width, height);
	}
	else {
		//qDebug() << "ASDFASDF";
		postRenderer->getDefaultFBO()->bind();
		glViewport(0, 0, width, height);
	}
	//COMBINE EVERYTHING(INCLUDING LIGHT)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
	combineShader->start();
	combineShader->loadInt("gAlbedoSpec", 2);
	combineShader->loadInt("gLight", 3);
	combineShader->loadFloat("exposure", exposure);
	gBuffer->bindColorAttachment(2);
	gBuffer->bindColorAttachment(3);
	//glBindVertexArray(screenVAO);
	screenVAO->bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DeferredRenderer::doDebugPass(Camera* cam, Entity* entity)
{
	//DEBUG RENDERER
	//debugRenderer->glBegin(GL_LINES);
	//debugRenderer->glColor3f(1, 0, 0);
	/*for (int i = 0; i < entity->model->meshes.size(); i++) {
		std::vector<glm::vec3> segs = entity->model->meshes[i]->transformedAabb.getLineSegments();
		for (int j = 0; j < segs.size(); j++) debugRenderer->glVertex3f(segs[j]);
	}*/
	//std::vector<glm::vec3> segs = cam->frustum->getLineSegments();
	//for (int i=0; i<segs.size();i++) debugRenderer->glVertex3f(segs[i]);

	//debugRenderer->glEnd(cam);
}

void DeferredRenderer::doSSAO(Camera *cam)
{
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	ssao->calculateSSAO(gBuffer.get(), screenVAO.get(), cam->getProjMatrix(), cam->getViewMatrix());
	
}

void DeferredRenderer::doPostProcessing(Camera* cam)
{
	if (postRenderer->getActiveEffectsCount() == 0) return;
	postRenderer->render(cam, gBuffer.get(), screenVAO.get());
}
