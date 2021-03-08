#include "postprocessingrenderer.h"

float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

PostProcessingRenderer::PostProcessingRenderer(int screenWidth, int screenHeight)
{
    initializeOpenGLFunctions();
    setupScreenQuad();
    mainFBO = std::make_unique<FrameBuffer>(1, true, screenWidth, screenHeight);
    postShader = ResourceManager::getInstance().load<ShaderProgram>("grayscale");
    postShader->start();
    postShader->loadInt("screenTexture", 0);
}

void PostProcessingRenderer::setupScreenQuad() {
    
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices[0], GL_STATIC_DRAW);

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

void PostProcessingRenderer::setViewport(int x, int y, int width, int height)
{
    mainFBO->setViewport(x, y, width, height);
    this->width = width;
    this->height = height;
    this->xS = x;
    this->yS = y;
}

PostProcessingRenderer::~PostProcessingRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void PostProcessingRenderer::startPostRenderTarget()
{
    // first pass
    mainFBO->bind();
    glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, mainFBO->getWidth(), mainFBO->getHeight());
    //DrawScene();
}

void PostProcessingRenderer::renderToScreen()
{
    // second pass
    mainFBO->unbind();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    postShader->start();
    glBindVertexArray(VAO);
    glDisable(GL_DEPTH_TEST);
    std::vector<unsigned int> colorAtts = mainFBO->getColorAttachments();
    if (colorAtts.size() > 0) {
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, colorAtts[0]);
    }
    glViewport(xS, yS, width, height);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
