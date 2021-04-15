#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <vector>
#include <memory>
#include <cstdarg>

struct TextureAttachment {
	GLenum dataType; //ex GL_FLOAT, GL_UNSIGNED_BYTE
	GLint internalFormat; //ex GL_RGBA16F, GL_RGBA
	GLenum format; //ex GL_RGBA
	GLint minMagFilter; // ex GL_LINEAR, GL_NEAREST
	GLuint texture = 0;
	unsigned int attachmentID = 0;
	bool depth = false;


};

class FrameBuffer : protected QOpenGLExtraFunctions {
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();
	
	void bind();
	void unbind();
	std::vector<std::unique_ptr<TextureAttachment>> const& getColorAttachments() const;
	std::unique_ptr<TextureAttachment> const& getDepthAttachment() const;
	void setViewport(int xS, int yS, int width, int height);
	unsigned int getWidth();
	unsigned int getHeight();
	void registerColorAttachment(unsigned int attachmentID, GLenum dataType, GLint internalFormat, GLenum format, GLint minMag);
	void registerDepthAttachment(GLenum dataType, GLint internalFormat, GLenum format, GLint minMag);
	void setup();
	void setRenderTargets(int, ...);
	void bindAllColorAttachments();
	void bindColorAttachment(int id);
	void bindColorAttachmentAtUnit(int id, int textureUnit);
	void bindDepthAttachment(int textureUnit);
private:
	unsigned int fbo = 0;
	void createFBO();
	//void registerAttachments(int colorAttCount, bool depth);
	void createTexAttachment(TextureAttachment *depthAttachment);
	bool setupDone = false;
	int width = 0;
	int height = 0;
	int xS = 0, yS = 0;
	std::vector<GLenum> renderTargets;
	//unsigned int colorAttSize = 0;
	//bool depthRequested = false;
	//std::vector<unsigned int> colorAtts;
	//unsigned int depthAtt = 0;
	std::vector<std::unique_ptr<TextureAttachment>> colorAttachments;
	std::unique_ptr<TextureAttachment> depthAttachment;
};
#endif // !FRAMEBUFFER_H
