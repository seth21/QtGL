#ifndef TEXTURE_H
#define TEXTURE_H
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "resource.h"

class Texture : public Resource, protected QOpenGLExtraFunctions
{
public:
    Texture(const std::string& fileName);
    void bind(int texUnit);
    void unbind();
    ~Texture();
    const std::string& getFullPath();
    bool loaded();
    void release();
    
private:
    unsigned char * mp_textureData;
    unsigned int m_texture, m_texUnit;
    int m_width, m_height, m_nrChannels;
    bool uploadGL();
    std::string fullPath;
    bool fileOK;
};

#endif // TEXTURE_H
