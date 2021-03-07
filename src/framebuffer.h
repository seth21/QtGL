#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <vector>

class FrameBuffer : protected QOpenGLExtraFunctions {
public:
	FrameBuffer(int colorAttachments, bool depthAttachment, int width, int height);
	~FrameBuffer();
	
	void bind();
	void unbind();
	std::vector<unsigned int> getColorAttachments();
	unsigned int getDepthAttachment();
	void setViewport(int xS, int yS, int width, int height);
	unsigned int getWidth();
	unsigned int getHeight();
private:
	unsigned int fbo = 0;
	void createFBO();
	void setup();
	void registerAttachments(int colorAttCount, bool depth);
	void createTexAttachment(unsigned int& textureHandle, unsigned int attachmentID, bool depth);
	int width = 0;
	int height = 0;
	int xS = 0, yS = 0;
	unsigned int colorAttSize = 0;
	bool depthRequested = false;
	std::vector<unsigned int> colorAtts;
	unsigned int depthAtt = 0;
};
#endif // !FRAMEBUFFER_H
