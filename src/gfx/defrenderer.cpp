#include "defrenderer.h"

DefRenderer::DefRenderer(int x, int y, int width, int height)
{
	f = QOpenGLContext::currentContext()->extraFunctions();
	m_xS = x;
	m_yS = y;
	m_width = width;
	m_height = height;
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
	//COMBINED
	gBuffer->registerColorAttachment(4, GL_FLOAT, GL_RGBA16F, GL_RGBA, GL_LINEAR, "combined");
	//DEPTH
	gBuffer->registerDepthAttachment(GL_UNSIGNED_BYTE, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_NEAREST, "depth");
	gBuffer->setRenderTargets(5, 0, 1, 2, 3, 4);
	gBuffer->setup();

	//ResourceConfig shaderConfig2;
	//shaderConfig2.addFlag("ALBEDO");
	//shaderConfig2.addFlag("BUMP");
	//gBufferShader2 = ResourceManager::getInstance().load<ShaderProgram>("gbuffer", shaderConfig2);


	dirLightShader = ResourceManager::getInstance().load<ShaderProgram>("gdirlight");
	pointLightShader = ResourceManager::getInstance().load<ShaderProgram>("gpointlight");
	combineShader = ResourceManager::getInstance().load<ShaderProgram>("gfinal");
	unitSphere = ResourceManager::getInstance().load<Model>("models/uniticosphere.obj");

}

DefRenderer::~DefRenderer()
{
}

void DefRenderer::setViewport(int x, int y, int width, int height)
{
	m_xS = x;
	m_yS = y;
	m_width = width;
	m_height = height;
	gBuffer->setViewport(x, y, width, height);
}

void DefRenderer::clearQueue()
{
	deferredQueue.clear();
	dirLights.clear();
	pointLights.clear();
}

void DefRenderer::doGeometryPass(Camera* cam)
{
	//GEOMETRY PASS-----------
	gBuffer->bind();
	gBuffer->setRenderTargets(3, 0, 1, 2);
	
	//GEOMETRY PASS
	f->glEnable(GL_DEPTH_TEST);
	f->glDepthMask(GL_TRUE);
	f->glDepthFunc(GL_LEQUAL);
	f->glEnable(GL_CULL_FACE);
	f->glCullFace(GL_BACK);
	f->glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak into g-buffer
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	f->glViewport(0, 0, gBuffer->getWidth(), gBuffer->getHeight());
	
	//DRAW ENTITIES
	for (auto &command : deferredQueue.getCommands()) {
		Material* mat = command.material;
		if (mat && mat->shader) {
			auto *shader = mat->shader.get();
			if (shader->start()) {
				//Global uniforms
				shader->loadMatrix4f("viewMat", cam->getViewMatrix());
				shader->loadMatrix4f("projMat", cam->getProjMatrix());
				shader->loadVector3f("viewPos", cam->position);
			}
				
			//Per material uniforms
			int counter = 0;
			for (auto tex : mat->getTextures()) {
				shader->loadInt(tex.first, counter);
				tex.second->bind(counter);
				counter++;
			}
			for (auto f : mat->getFloats()) {
				shader->loadFloat(f.first, f.second);
			}
			for (auto v3 : mat->getVec3s()) {
				shader->loadVector3f(v3.first, v3.second);
			}
			//Per object uniforms
			shader->loadMatrix4f("modelMat", command.transformMatrix);
			f->glBindVertexArray(command.vaoID);
			f->glDrawElements(GL_TRIANGLES, command.vertexCount, GL_UNSIGNED_INT, (GLvoid*)command.baseVertex);
		}
	}
}

CommandQueue& DefRenderer::getQueue()
{
	return deferredQueue;
}

FrameBuffer* DefRenderer::getGBuffer()
{
	return gBuffer.get();
}

VAO* DefRenderer::getScreenVAO()
{
	return screenVAO.get();
}

void DefRenderer::addDirectionalLight(DirectionalLight* dirLight)
{
	dirLights.push_back(dirLight);
}

void DefRenderer::addPointLight(PointLight* pointLight)
{
	pointLights.push_back(pointLight);
}

void DefRenderer::doDirectionalLightPass(Camera* cam, SSAO* ssao)
{
	
	for (auto dirLight : dirLights) {
		//DIRECTIONAL LIGHT
		f->glDisable(GL_DEPTH_TEST);
		f->glCullFace(GL_BACK);
		f->glDepthMask(GL_TRUE);
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
		dirLightShader->loadVector3f("ambientLight", dirLight->getColor()*0.08f);
		dirLightShader->loadVector3f("lightColor", dirLight->getColor());
		dirLightShader->loadVector3f("viewPos", cam->position);
		dirLightShader->loadInt("shadowMap", 3);
		dirLight->getShadowMap()->bindDepthAttachment(3);
		dirLightShader->loadMatrix4f("shadowMapSpaceMatrix", dirLight->getToShadowMapSpaceMatrix());
		dirLightShader->loadFloat("shadowDistance", 250);
		dirLightShader->loadFloat("transitionDistance", 220);
		//ssao
		dirLightShader->loadInt("ssaoTexture", 4);
		ssao->bindResult(4);
		f->glViewport(0, 0, gBuffer->getWidth(), gBuffer->getHeight());
		//Render Directional Light as a screen-sized Quad to render attachment 3
		//glBindVertexArray(screenVAO);
		screenVAO->bind();
		f->glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
}

void DefRenderer::doPointLightPass(Camera* cam)
{
	//POINT LIGHTS
	f->glEnable(GL_DEPTH_TEST);
	f->glDepthFunc(GL_GEQUAL);
	f->glCullFace(GL_FRONT);
	f->glDepthMask(GL_FALSE);
	f->glEnable(GL_BLEND);
	f->glBlendFunc(GL_ONE, GL_ONE);

	pointLightShader->start();
	pointLightShader->loadMatrix4f("pv", cam->getPVmatrix());
	pointLightShader->loadInt("gPosition", 0);
	pointLightShader->loadInt("gNormal", 1);
	pointLightShader->loadInt("gAlbedoSpec", 2);
	gBuffer->bindColorAttachment(0);
	gBuffer->bindColorAttachment(1);
	gBuffer->bindColorAttachment(2);
	pointLightShader->loadVector2f("gBufferSize", glm::vec2(gBuffer->getWidth(), gBuffer->getHeight()));
	pointLightShader->loadVector3f("viewPos", cam->position);
	Mesh* sphereMesh = unitSphere->meshes[0];

	for (auto pointLight : pointLights) {
		glm::vec3& position = pointLight->position;
		glm::vec3& lightColor = pointLight->lightColor;
		float constant = pointLight->constant;
		float linear = pointLight->linear;
		float quadratic = pointLight->quadratic;
		
		pointLightShader->loadFloat("light.constant", constant);
		pointLightShader->loadFloat("light.linear", linear);
		pointLightShader->loadFloat("light.quadratic", quadratic);
		pointLightShader->loadVector3f("light.color", lightColor);
		pointLightShader->loadVector3f("light.position", position);
		
		float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
		float radius =
			(-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (exposure * 256.0 / 3.0) * lightMax)))
			/ (2 * quadratic);
		pointLightShader->loadFloat("light.maxRadius", radius);
		
		float sphereScale = radius * 1.03f; //extend the radius slightly so we dont see the sphere's triangles
		glm::mat4 scl = glm::scale(glm::mat4(1.0f), glm::vec3(sphereScale));
		glm::mat4 trans = glm::translate(glm::mat4(1.0), position);
		pointLightShader->loadMatrix4f("modelMat", trans * scl);
		
		f->glBindVertexArray(sphereMesh->VAO);
		f->glDrawElements(GL_TRIANGLES, sphereMesh->indices.size(), GL_UNSIGNED_INT, 0);
		
	}
	f->glDisable(GL_BLEND);
}

void DefRenderer::doDeferredLighting(Camera* cam, SSAO* ssao)
{
	gBuffer->bind();
	gBuffer->setRenderTargets(1, 3); //Render to the light target only
	f->glClear(GL_COLOR_BUFFER_BIT);
	doDirectionalLightPass(cam, ssao);
	doPointLightPass(cam);
}

void DefRenderer::doCombinePass()
{
	gBuffer->bind();
	gBuffer->setRenderTargets(1, 4); //Render to the combined target only
	//COMBINE EVERYTHING(INCLUDING LIGHT)
	f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	f->glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	f->glCullFace(GL_BACK);
	combineShader->start();
	combineShader->loadInt("gAlbedoSpec", 2);
	combineShader->loadInt("gLight", 3);
	combineShader->loadFloat("exposure", exposure);
	gBuffer->bindColorAttachment(2);
	gBuffer->bindColorAttachment(3);
	
	screenVAO->bind();
	f->glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DefRenderer::setExposure(float exp)
{
	exposure = exp;
}
