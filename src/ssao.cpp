#include "ssao.h"

//Calculate SSAO on half size buffer
SSAO::SSAO(int width, int height)
{
    f = QOpenGLContext::currentContext()->extraFunctions();
    ssaoShader = ResourceManager::getInstance().load<ShaderProgram>("ssao");
    blurShader = ResourceManager::getInstance().load<ShaderProgram>("gaussianblur");
    generateKernelNoise();
    noiseTexture = std::make_unique<Texture>();
    noiseTexture->uploadFloat2D(4, 4, ssaoNoise.data(), GL_RGBA16F, GL_RGB, GL_FLOAT, GL_NEAREST, GL_REPEAT);
    
    ssaoBuffer = std::make_unique<FrameBuffer>(width, height);
    ssaoBuffer->registerColorAttachment(0, GL_FLOAT, GL_RED, GL_RED, GL_LINEAR, "ssao");
    ssaoBuffer->setup();
    
    blurBuffer = std::make_unique<FrameBuffer>(width/2, height/2);
    //1
    blurBuffer->registerColorAttachment(0, GL_FLOAT, GL_RED, GL_RED, GL_LINEAR, "blur1");
    //2
    blurBuffer->registerColorAttachment(1, GL_FLOAT, GL_RED, GL_RED, GL_LINEAR, "blur2");
    blurBuffer->setup();
    
    ssaoShader->start();
    ssaoShader->loadInt("gPosition", 0);
    ssaoShader->loadInt("gNormal", 1);
    ssaoShader->loadInt("noiseTexture", 2);
    for (unsigned int i = 0; i < 64; ++i)
        ssaoShader->loadVector3f("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
    ssaoShader->stop();
}

SSAO::~SSAO()
{
}

//Bind gPosition to texUnit=0, gNormal to texUnit=1
void SSAO::calculateSSAO(FrameBuffer* gBuffer, VAO* screenVAO, glm::mat4 projection, glm::mat4 view)
{
    //GL state
    f->glDisable(GL_DEPTH_TEST);
    f->glCullFace(GL_BACK);
    ssaoBuffer->bind();
    ssaoBuffer->setRenderTargets(1, 0);
    f->glClear(GL_COLOR_BUFFER_BIT);
    //Bind GBuffer/noise Textures
    ssaoShader->start();

    ssaoShader->loadMatrix4f("projection", projection);
    ssaoShader->loadMatrix4f("view", view);
    
    gBuffer->bindColorAttachmentAtUnit("position", 0);
    gBuffer->bindColorAttachmentAtUnit("normal", 1);
    noiseTexture->bind(2);
    
    screenVAO->bind();
    f->glViewport(0, 0, ssaoBuffer->getWidth(), ssaoBuffer->getHeight());
    f->glDrawArrays(GL_TRIANGLES, 0, 6);

    //Blur result
    int blurIterations = 1;
    blurBuffer->bind();
    blurShader->start();
    f->glViewport(0, 0, blurBuffer->getWidth(), blurBuffer->getHeight());
    for (int i = 0; i < blurIterations; i++) {
        //Horizontal blur
        blurBuffer->setRenderTargets(1, 0);
        blurShader->loadFloat("horizontal", 1);
        if (i == 0) ssaoBuffer->bindColorAttachment(0);
        else blurBuffer->bindColorAttachmentAtUnit(1, 0);
        blurShader->loadInt("mainTexture", 0);
        f->glDrawArrays(GL_TRIANGLES, 0, 6);
        //Vertical blur
        blurBuffer->setRenderTargets(1, 1);
        blurShader->loadFloat("horizontal", 0);
        blurBuffer->bindColorAttachmentAtUnit(0, 0);
        blurShader->loadInt("mainTexture", 0);
        f->glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
}

void SSAO::bindResult(int textureUnit)
{
    blurBuffer->bindColorAttachmentAtUnit(1, textureUnit);
}

void SSAO::screenSizeChanged(int width, int height)
{
    ssaoBuffer->setViewport(0, 0, width, height);
    blurBuffer->setViewport(0, 0, width / 2, height / 2);
}

float SSAO::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

void SSAO::generateKernelNoise()
{
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
    std::default_random_engine generator;

    //generate random samples around a hemisphere
    //for (unsigned int i = 0; i < 64; ++i)
    while (ssaoKernel.size() < 64)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample = glm::normalize(sample);
        if (glm::dot(sample, glm::vec3(0, 0, 1)) < 0.15f) continue;
        sample *= randomFloats(generator);
        float scale = float(ssaoKernel.size()) / 64.0;

        // scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.2f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    //Generate noise
    for (unsigned int i = 0; i < 16; i++)
    {
        ssaoNoise.push_back(randomFloats(generator) * 2.0 - 1.0);
        ssaoNoise.push_back(randomFloats(generator) * 2.0 - 1.0);
        ssaoNoise.push_back(0);
    }
}


