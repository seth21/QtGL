#ifndef TEXTURE_H
#define TEXTURE_H
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

class Texture : protected QOpenGLExtraFunctions
{
public:
    Texture(std::string fileName);
    void bind(int texUnit);
    void unbind();
    ~Texture();
    const std::string& getFullPath();
    bool fileLoaded();

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
