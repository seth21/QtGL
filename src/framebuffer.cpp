#include "framebuffer.h"

FrameBuffer::FrameBuffer(int width, int height) {
	initializeOpenGLFunctions();

	this->width = width;
	this->height = height;
	qDebug() << "New FBO: w/h:" << width << " " << height;
	createFBO();
	//registerAttachments(colorAttachments, depthAttachment);
	if (width == 0 || height == 0) {
		qDebug() << "Invalid Framebuffer width/height is 0!";
		return;
	}
	//setup();
	
	
}

FrameBuffer::~FrameBuffer()
{
	unbind();
	glDeleteFramebuffers(1, &fbo);
	//delete textures
	if (depthAttachment) glDeleteTextures(1, &depthAttachment->texture);
	for (int i = 0; i < colorAttachments.size(); i++) glDeleteTextures(1, &colorAttachments[i]->texture);
}

void FrameBuffer::setup()
{
	if (width == 0 || height == 0) {
		qDebug() << "Cannot initialize Framebuffer width/height is 0!";
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//Destroy old textures, if any
	if (depthAttachment && depthAttachment->texture != 0) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, NULL, 0);
		glDeleteTextures(1, &(depthAttachment->texture));
		depthAttachment->texture = 0;
	}
	for (int i = 0; i < colorAttachments.size(); i++) {
		if (colorAttachments[i]->texture == 0) continue; //Non initialized attachment, nothing to delete
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, NULL, 0);
		glDeleteTextures(1, &(colorAttachments[i]->texture));
		colorAttachments[i]->texture = 0;
	}
	//Create new textures
	for (int i = 0; i < colorAttachments.size(); i++) {
		createTexAttachment(colorAttachments[i].get());
	}
	if (renderTargets.size() > 0) {
		// set multiple render targets 
		qDebug() << "MRT ENABLED";
		//std::vector<unsigned int> attachments;
		//for (int i = 0; i < renderTargetCount; i++) attachments.push_back(GL_COLOR_ATTACHMENT0 + renderTargets[i]);
		//unsigned int attach[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(renderTargets.size(), &renderTargets[0]);
	}
	if (depthAttachment) createTexAttachment(depthAttachment.get());
	//unbind();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		qDebug() << "Framebuffer could not be set!";
	else {
		setupDone = true;
		//int depthCount = (depthAtt != 0) ? 1 : 0;
		qDebug() << "Initialized frame buffer with color attachments:" << colorAttachments.size() << ", depth attachments:" << (depthAttachment != nullptr);
	}
	
	unbind();
}

//The first integer is the number of the targets, and then the attachment ids
void FrameBuffer::setRenderTargets(int num, ...)
{
	va_list arguments;                    

	va_start(arguments, num);           
	for (int x = 0; x < num; x++)        
		renderTargets.push_back(GL_COLOR_ATTACHMENT0 + va_arg(arguments, int)); 
	va_end(arguments);
}

//Binds all color attachments using the attachment id as the texture unit.
void FrameBuffer::bindAllColorAttachments()
{
	for (int i = 0; i < colorAttachments.size(); i++) {
		unsigned int texUnit = GL_TEXTURE0 + colorAttachments[i]->attachmentID;
		GLuint texHandle = colorAttachments[i]->texture;
		glActiveTexture(texUnit);
		glBindTexture(GL_TEXTURE_2D, texHandle);
	}
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (!setupDone) setup();
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::vector<std::unique_ptr<TextureAttachment>> const& FrameBuffer::getColorAttachments() const
{
	return colorAttachments;
}

std::unique_ptr<TextureAttachment> const& FrameBuffer::getDepthAttachment() const
{
	return depthAttachment;
}

void FrameBuffer::createTexAttachment(TextureAttachment* attachment)
{
	glGenTextures(1, &(attachment->texture));
	glBindTexture(GL_TEXTURE_2D, attachment->texture);

	glTexImage2D(GL_TEXTURE_2D, 0, attachment->internalFormat, width, height, 0, attachment->format, attachment->dataType, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, attachment->minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, attachment->minMagFilter);
	//Attach the texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, (attachment->depth) ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + attachment->attachmentID, GL_TEXTURE_2D, attachment->texture, 0);
}

void FrameBuffer::registerColorAttachment(unsigned int attachmentID, GLenum dataType, GLint internalFormat, GLenum format, GLint minMag)
{
	std::unique_ptr<TextureAttachment> att = std::make_unique<TextureAttachment>();
	for (int i = 0; i < colorAttachments.size(); i++) {
		if (colorAttachments[i]->attachmentID == attachmentID) qDebug() << "FBO - Color attachment with id:" << attachmentID << " already exists!";
	}
	att->depth = false;
	att->attachmentID = attachmentID;
	att->dataType = dataType;
	att->internalFormat = internalFormat;
	att->format = format;
	att->minMagFilter = minMag;
	colorAttachments.push_back(std::move(att));
}

void FrameBuffer::registerDepthAttachment(GLenum dataType, GLint internalFormat, GLenum format, GLint minMag)
{
	depthAttachment = std::make_unique<TextureAttachment>();
	depthAttachment->depth = true;
	depthAttachment->attachmentID = 0;
	depthAttachment->dataType = dataType;
	depthAttachment->internalFormat = internalFormat;
	depthAttachment->format = format;
	depthAttachment->minMagFilter = minMag;
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
