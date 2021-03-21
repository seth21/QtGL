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
	setupScreenQuad();
	gBuffer = std::make_unique<FrameBuffer>(width, height);
	//POSITIONS
	gBuffer->registerColorAttachment(0, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_NEAREST);
	//NORMALS
	gBuffer->registerColorAttachment(1, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_NEAREST);
	//COLOR+SPEC
	gBuffer->registerColorAttachment(2, GL_UNSIGNED_BYTE, GL_RGBA, GL_RGBA, GL_NEAREST);
	//DEPTH
	gBuffer->registerDepthAttachment(GL_UNSIGNED_BYTE, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_NEAREST);
	gBuffer->setRenderTargets(3, 0, 1, 2);
	gBuffer->setup();
	qDebug() << "Shader gbuffer";
	ResourceConfig shaderConfig;
	shaderConfig.flags.push_back("ALBEDO");
	gBufferShader = ResourceManager::getInstance().load<ShaderProgram>("gbuffer", shaderConfig);
	qDebug() << "Shader dirlight";
	dirLightShader = ResourceManager::getInstance().load<ShaderProgram>("gdirlight");
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::render(Camera* cam, Entity* entity)
{
	//GEOMETRY PASS
	gBuffer->bind();
	glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak into g-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, gBuffer->getWidth(), gBuffer->getHeight());
	gBufferShader->start();
	gBufferShader->loadMatrix4f("viewMat", cam->getViewMatrix());
	gBufferShader->loadMatrix4f("projMat", cam->getProjMatrix());
	/*
	for(Object obj : Objects)
    {
        ConfigureShaderTransformsAndUniforms();
        obj.Draw();
    }  */
	entity->drawNow(gBufferShader.get(), cam);

	gBuffer->unbind();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	dirLightShader->start();
	//Bind GBuffer Textures
	dirLightShader->loadInt("gPosition", 0);
	dirLightShader->loadInt("gNormal", 1);
	dirLightShader->loadInt("gAlbedoSpec", 2);
	gBuffer->bindAllColorAttachments();
	//Lighting Uniforms
	dirLightShader->loadVector3f("lightPos", glm::vec3(0, 90, 20));
	dirLightShader->loadVector3f("ambientLight", glm::vec3(0.2, 0.2, 0.1));
	dirLightShader->loadVector3f("lightColor", glm::vec3(0.7, 0.6, 0.3));
	dirLightShader->loadVector3f("viewPos", cam->position);
	//Render Quad
	glBindVertexArray(VAO);
	glViewport(xS, yS, width, height);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DeferredRenderer::setViewport(int x, int y, int width, int height)
{
	gBuffer->setViewport(x, y, width, height);
	this->width = width;
	this->height = height;
	this->xS = x;
	this->yS = y;
}

void DeferredRenderer::setupScreenQuad() {

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
}
