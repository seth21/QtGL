#include "skyrenderer.h"

const float SkyRenderer::cubeVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

SkyRenderer::SkyRenderer() {
    initializeOpenGLFunctions();
    generateCubeMesh();
    ResourceConfig cubeConfig;
    cubeConfig.flags.push_back("cube");
    skyShader = ResourceManager::getInstance().load<ShaderProgram>("skybox", cubeConfig);
    skyShader->start();
    skyShader->loadInt("skybox", 0);
}

SkyRenderer::~SkyRenderer() {

}

void SkyRenderer::generateCubeMesh()
{
    cubeVAO = std::make_unique<VAO>();
    int vaoPos = cubeVAO->createAttribute(VertexAttrib::POSITION, 0, 3, cubeVertices, 108);
    cubeVAO->upload();
}

void SkyRenderer::render(Camera* cam, FrameBuffer* target)
{
    if (!skyTexture || !skyShader) return;
    target->bind();
    target->setRenderTargets(1, 4);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);
    //glDepthMask(GL_FALSE);
    //Do not write to the depth buffer
    
    skyShader->start();
    
    skyShader->loadMatrix4f("projection", cam->getProjMatrix());
    //ignore camera's translation
    glm::mat4 view = glm::mat4(glm::mat3(cam->getViewMatrix()));
    skyShader->loadMatrix4f("view", view);
    skyTexture->bind(0);
    cubeVAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDepthMask(GL_TRUE);
    glDisable(GL_DEPTH_TEST);
}

void SkyRenderer::setSkyTexture(std::shared_ptr<Texture> skyTex)
{
    skyTexture = skyTex;
}
