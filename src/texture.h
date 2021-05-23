#ifndef TEXTURE_H
#define TEXTURE_H
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QOpenGLExtraFunctions>
#include "resource.h"
#include "resourceconfig.h"
#include <glm/vec3.hpp>

class Texture : public Resource
{
public:
    Texture(const std::string& fileName, ResourceConfig config);
    Texture();
    virtual void bind(int texUnit);
    virtual void unbind();
    ~Texture();
    const std::string& getFullPath();
    bool loaded();
    void release();
    ResourceConfig& getResourceConfig();
    void uploadFloat2D(int width, int height, const float* data, GLint internalFormat, GLenum format, GLenum type, GLenum filter, GLenum wrap);
    void uploadFloat3D(int faceWidth, int faceHeight, const float* faceData[6], GLint internalFormat, GLenum format, GLenum type, GLenum filter, GLenum wrap);
    unsigned int getHandle();
    unsigned int getChannelsCount();
private:
    unsigned char * mp_textureData;
    unsigned int m_texture, m_texUnit;
    int m_width, m_height, m_nrChannels;
    void loadImage(const std::string& fileName);
    void loadCubeImage(const std::string& fileName);
    std::string fullPath;
    bool fileOK;
    bool cubeTexture = false;
    ResourceConfig resConfig;
    std::vector<std::string> splitString(const std::string& s, const std::string& delim);
    QOpenGLExtraFunctions* gl;
};

#endif // TEXTURE_H
