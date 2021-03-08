#include "framebuffer.h"

FrameBuffer::FrameBuffer(int colorAttachments, bool depthAttachment, int width, int height) {
	initializeOpenGLFunctions();

	this->width = width;
	this->height = height;
	qDebug() << "New FBO: w/h:" << width << " " << height;
	createFBO();
	registerAttachments(colorAttachments, depthAttachment);
	if (width == 0 || height == 0) {
		qDebug() << "Invalid Framebuffer width/height is 0!";
		return;
	}
	setup();
	
	
}

FrameBuffer::~FrameBuffer()
{
	unbind();
	glDeleteFramebuffers(1, &fbo);
	//delete textures
	if (depthAtt != 0) glDeleteTextures(1, &depthAtt);
	for (int i = 0; i < colorAtts.size(); i++) glDeleteTextures(1, &colorAtts[i]);
}

void FrameBuffer::setup()
{
	if (width == 0 || height == 0) {
		qDebug() << "Cannot initialize Framebuffer width/height is 0!";
		return;
	}
	bind();
	//Destroy old textures, if any
	if (depthRequested && depthAtt != 0) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, NULL, 0);
		glDeleteTextures(1, &depthAtt);
		depthAtt = 0;
	}
	for (int i = 0; i < colorAtts.size(); i++) {
		if (colorAtts[i] == 0) continue; //Non initialized attachment, nothing to delete
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, NULL, 0);
		glDeleteTextures(1, &colorAtts[i]);
		colorAtts[i] = 0;
	}
	//Create new textures
	for (int i = 0; i < colorAtts.size(); i++) {
		createTexAttachment(colorAtts[i], 0, false);
	}
	if (depthRequested) createTexAttachment(depthAtt, 0, true);
	//unbind();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		qDebug() << "Framebuffer could not be set!";
	else {
		int depthCount = (depthAtt != 0) ? 1 : 0;
		//qDebug() << "Initialized frame buffer with color attachments:" << colorAtts.size() << ", depth attachments:" << depthCount;
	}
	
	unbind();
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::vector<unsigned int> FrameBuffer::getColorAttachments()
{
	return colorAtts;
}

unsigned int FrameBuffer::getDepthAttachment()
{
	return depthAtt;
}

void FrameBuffer::registerAttachments(int colorAttCount, bool depth)
{
	if (depth) depthRequested = true;
	for (int i = 0; i < colorAttCount; i++) colorAtts.push_back(0);
}

void FrameBuffer::createTexAttachment(unsigned int &textureHandle, unsigned int attachmentIndex, bool depth)
{
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, depth ? GL_DEPTH_COMPONENT : GL_RGB, width, height, 0, depth ? GL_DEPTH_COMPONENT : GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Attach the texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, depth ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, textureHandle, 0);
}

void FrameBuffer::setViewport(int x, int y, int width, int height)
{
	this->width = width;
	this->height = height;
	this->xS = x;
	this->yS = y;
	if (width == 0 || height == 0) {
		qDebug() << "Invalid Framebuffer width/height is 0!";
		return;
	}
	setup();
}

unsigned int FrameBuffer::getWidth()
{
	return width;
}

unsigned int FrameBuffer::getHeight()
{
	return height;
}

void FrameBuffer::createFBO()
{
	glGenFramebuffers(1, &fbo);
}
