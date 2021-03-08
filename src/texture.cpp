#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string &fileName, ResourceConfig config) : Resource(fileName, config)
{
    resConfig = config;
    cubeTexture = resConfig.flagExists("cube");
    qDebug() << "Texture cube:" << cubeTexture;

    if (!cubeTexture) loadImage(fileName);
    else loadCubeImage(fileName);
    
}

void Texture::loadCubeImage(const std::string& fileName) {
    std::vector<std::string> splits = splitString(fileName, "*");
    if (splits.size() != 7) {
        qDebug() << "Failed to load cube texture - need 6 textures";
        fileOK = false;
        return;
    }
    initializeOpenGLFunctions();
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    
    for (int i = 1; i < splits.size(); i++) {
        QDir imagePath = QDir(QApplication::applicationDirPath() + "/resources/" + splits[0].c_str() + splits[i].c_str());
        mp_textureData = stbi_load(imagePath.absolutePath().toUtf8(), &m_width, &m_height, &m_nrChannels, 0);
        if (!mp_textureData) {
            qDebug() << "Failed to load cube texture data:" << imagePath.absolutePath().toStdString().c_str();
            fileOK = false;
            return;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i - 1, 0, m_nrChannels == 4 ? GL_RGBA : GL_RGB, m_width, m_height, 0, m_nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, mp_textureData);
        qDebug() << "Loaded cube texture:" << imagePath.absolutePath() << "->" << sizeof(mp_textureData) << m_width << m_height << m_nrChannels;
        stbi_image_free(mp_textureData);

    }
    qDebug() << "Loaded cube texture:" << fileName.c_str();
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    fileOK = true;
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture::loadImage(const std::string& fileName) {
    std::vector<std::string> splits = splitString(fileName, "%");
    QDir imagePath = QDir(QApplication::applicationDirPath() + "/resources/" + fileName.c_str());
    mp_textureData = stbi_load(imagePath.absolutePath().toUtf8(), &m_width, &m_height, &m_nrChannels, 0);
    qDebug() << "Loaded texture:" << fileName.c_str() << "->" << sizeof(mp_textureData) << m_width << m_height << m_nrChannels;
    fullPath = imagePath.absolutePath().toStdString();
    if (!mp_textureData) {
        qDebug() << "Failed to load texture";
        fileOK = false;
        return;
    }
    initializeOpenGLFunctions();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, m_nrChannels == 4 ? GL_RGBA : GL_RGB, m_width, m_height, 0, m_nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, mp_textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(mp_textureData);
    fileOK = true;
}

void Texture::bind(int texUnit){
    m_texUnit = GL_TEXTURE0 + texUnit;
    glActiveTexture(m_texUnit);
    glBindTexture(cubeTexture ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_texture);
}

void Texture::unbind(){
    glActiveTexture(m_texUnit);
    glBindTexture(cubeTexture ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
}

Texture::~Texture(){
    release();
}

const std::string& Texture::getFullPath()
{
    return fullPath;
}

ResourceConfig& Texture::getResourceConfig()
{
    return resConfig;
}

bool Texture::loaded()
{
    qDebug() << "Texture " << fileOK;
    return fileOK;
}

void Texture::release()
{
    unbind();
    glDeleteTextures(1, &m_texture);
}

std::vector<std::string> Texture::splitString(const std::string& s, const std::string& delim) {
    std::vector<std::string> splits;
    auto start = 0U;
    auto end = s.find(delim);
    while (end != std::string::npos)
    {
        splits.push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }

    splits.push_back(s.substr(start, end));
    return splits;
}

