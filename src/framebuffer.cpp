#include "framebuffer.h"

FrameBuffer::FrameBuffer(int width, int height) {
	f = QOpenGLContext::currentContext()->extraFunctions();

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
	f->glDeleteFramebuffers(1, &fbo);
	//delete textures
	if (depthAttachment) {
		//glDeleteTextures(1, &depthAttachment->texture);
		depthAttachment->tex.reset();
	}
	for (int i = 0; i < colorAttachments.size(); i++) {
		//glDeleteTextures(1, &colorAttachments[i]->texture);
		colorAttachments[i]->tex.reset();
	}
}

void FrameBuffer::setup()
{
	if (width == 0 || height == 0) {
		qDebug() << "Cannot initialize Framebuffer width/height is 0!";
		return;
	}
	f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//Destroy old textures, if any
	if (depthAttachment && depthAttachment->tex.get() != nullptr) {
		f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, NULL, 0);
		//glDeleteTextures(1, &(depthAttachment->texture));
		depthAttachment->tex.reset(nullptr);
		//depthAttachment->texture = 0;
	}
	for (int i = 0; i < colorAttachments.size(); i++) {
		if (colorAttachments[i]->tex.get() == nullptr) continue; //Non initialized attachment, nothing to delete
		f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, NULL, 0);
		//glDeleteTextures(1, &(colorAttachments[i]->texture));
		//colorAttachments[i]->texture = 0;
		colorAttachments[i]->tex.reset(nullptr);
		//qDebug() << "AAAAAAAAAAAAAAa";
	}
	//Create new textures
	for (int i = 0; i < colorAttachments.size(); i++) {
		createTexAttachment(colorAttachments[i].get());
	}
	if (renderTargets.size() > 0) {
		// set multiple render targets 
		//std::vector<unsigned int> attachments;
		//for (int i = 0; i < renderTargetCount; i++) attachments.push_back(GL_COLOR_ATTACHMENT0 + renderTargets[i]);
		//unsigned int attach[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		f->glDrawBuffers(renderTargets.size(), &renderTargets[0]);
	}
	else {
		f->glDrawBuffers(0, NULL);
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
	}
	if (depthAttachment) createTexAttachment(depthAttachment.get());
	//unbind();
	if (f->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
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
	renderTargets.clear();
	va_start(arguments, num);           
	for (int x = 0; x < num; x++)        
		renderTargets.push_back(GL_COLOR_ATTACHMENT0 + va_arg(arguments, int)); 
	va_end(arguments);
	if (setupDone) {
		if (num > 0) f->glDrawBuffers(renderTargets.size(), &renderTargets[0]);
		else {
			//glDrawBuffer(GL_NONE);
			//glReadBuffer(GL_NONE);
			f->glDrawBuffers(0, NULL);
		}
	}
}

//Binds all color attachments using the attachment id as the texture unit.
void FrameBuffer::bindAllColorAttachments()
{
	for (int i = 0; i < colorAttachments.size(); i++) {
		Texture* tex = colorAttachments[i]->tex.get();
		if (tex) tex->bind(colorAttachments[i]->attachmentID);
		//glActiveTexture(texUnit);
		//glBindTexture(GL_TEXTURE_2D, texHandle);
	}
}

void FrameBuffer::bindColorAttachment(int id)
{
	for (int i = 0; i < colorAttachments.size(); i++) {
		if (colorAttachments[i]->attachmentID == id) {
			Texture* tex = colorAttachments[i]->tex.get();
			if (tex) tex->bind(colorAttachments[i]->attachmentID);
			break;
		}
		//glActiveTexture(texUnit);
		//glBindTexture(GL_TEXTURE_2D, texHandle);
	}
}

void FrameBuffer::bindColorAttachmentAtUnit(std::string name, unsigned int textureUnit)
{
	for (int i = 0; i < colorAttachments.size(); i++) {
		if (colorAttachments[i]->name == name) {
			Texture* tex = colorAttachments[i]->tex.get();
			if (tex) tex->bind(textureUnit);
			break;
		}
		//glActiveTexture(texUnit);
		//glBindTexture(GL_TEXTURE_2D, texHandle);
	}
}

void FrameBuffer::bindColorAttachmentAtUnit(int id, int textureUnit)
{
	for (int i = 0; i < colorAttachments.size(); i++) {
		if (colorAttachments[i]->attachmentID == id)
		{
			Texture* tex = colorAttachments[i]->tex.get();
			if (tex) tex->bind(textureUnit);
			break;
		}
		
		//glActiveTexture(texUnit);
		//glBindTexture(GL_TEXTURE_2D, texHandle);
	}
}

void FrameBuffer::bindDepthAttachment(int textureUnit)
{
	if (!depthAttachment) return;
	//GLuint texHandle = depthAttachment->texture;
	Texture* tex = depthAttachment->tex.get();
	if (tex) tex->bind(textureUnit);
	//glActiveTexture(texUnit);
	//glBindTexture(GL_TEXTURE_2D, texHandle);
}

Texture* FrameBuffer::getColorAttachment(std::string name)
{
	for (int i = 0; i < colorAttachments.size(); i++) {
		if (colorAttachments[i]->name == name)
		{
			return colorAttachments[i]->tex.get();
		}
	}
}

void FrameBuffer::setClearColor(glm::vec3 color)
{
	clearColor = color;
}

glm::vec3 FrameBuffer::getClearColor()
{
	return clearColor;
}

void FrameBuffer::bind()
{
	f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (!setupDone) setup();
}

void FrameBuffer::unbind()
{
	f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	attachment->tex = std::make_unique<Texture>();
	attachment->tex->uploadFloat2D(width, height, nullptr, attachment->internalFormat, attachment->format, attachment->dataType, attachment->minMagFilter, GL_CLAMP_TO_EDGE);
	
	//glGenTextures(1, &(attachment->texture));
	//glBindTexture(GL_TEXTURE_2D, attachment->texture);

	//glTexImage2D(GL_TEXTURE_2D, 0, attachment->internalFormat, width, height, 0, attachment->format, attachment->dataType, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, attachment->minMagFilter);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, attachment->minMagFilter);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//Attach the texture to the FBO
	f->glFramebufferTexture2D(GL_FRAMEBUFFER, (attachment->depth) ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + attachment->attachmentID, GL_TEXTURE_2D, attachment->tex->getHandle(), 0);
}

void FrameBuffer::registerColorAttachment(unsigned int attachmentID, GLenum dataType, GLint internalFormat, GLenum format, GLint minMag, std::string name)
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
	att->name = name;
	colorAttachments.push_back(std::move(att));
}

void FrameBuffer::registerDepthAttachment(GLenum dataType, GLint internalFormat, GLenum format, GLint minMag, std::string name)
{
	depthAttachment = std::make_unique<TextureAttachment>();
	depthAttachment->depth = true;
	depthAttachment->attachmentID = 0;
	depthAttachment->dataType = dataType;
	depthAttachment->internalFormat = internalFormat;
	depthAttachment->format = format;
	depthAttachment->minMagFilter = minMag;
	depthAttachment->name = name;
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
	f->glGenFramebuffers(1, &fbo);
}

void FrameBuffer::clear()
{
	GLbitfield cl = GL_NONE;
	if (renderTargets.size() > 0 && colorAttachments.size() > 0) cl = GL_COLOR_BUFFER_BIT;
	if (depthAttachment) cl = cl | GL_DEPTH_BUFFER_BIT;
	f->glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0);
	f->glClear(cl);
}
