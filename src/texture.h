#ifndef TEXTURE_H
#define TEXTURE_H
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "resource.h"
#include "resourceconfig.h"

class Texture : public Resource, protected QOpenGLFunctions
{
public:
    Texture(const std::string& fileName, ResourceConfig config);
    virtual void bind(int texUnit);
    virtual void unbind();
    ~Texture();
    const std::string& getFullPath();
    bool loaded();
    void release();
    ResourceConfig& getResourceConfig();
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
};

#endif // TEXTURE_H
