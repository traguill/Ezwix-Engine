#include "Application.h"
#include "OpenGLFunc.h"

#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

unsigned int OpenGLFunc::CreateFrameBuffer()
{
	unsigned int frame_buffer;
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return frame_buffer;
}

unsigned int OpenGLFunc::CreateTextureAttachment(int width, int height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
}

unsigned int OpenGLFunc::CreateDepthTextureAttachment(int width, int height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}

unsigned int OpenGLFunc::CreateDepthBufferAttachment(int width, int height)
{
	unsigned int depth_buffer;
	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	return depth_buffer;
}

void OpenGLFunc::BindFrameBuffer(unsigned int frame_buffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glViewport(0, 0, width, height);
}

void OpenGLFunc::UnbindCurrentFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, App->window->GetScreenWidth(), App->window->GetScreenHeight());
}

void OpenGLFunc::DeleteFrameBuffer(unsigned int frame_buffer)
{
	glDeleteFramebuffers(1, &frame_buffer);
}

void OpenGLFunc::DeleteRenderBuffer(unsigned int render_buffer)
{
	glDeleteRenderbuffers(1, &render_buffer);
}

void OpenGLFunc::DeleteTexture(unsigned int texture)
{
	glDeleteTextures(1, &texture);
}

