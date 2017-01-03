#ifndef __OPENGLFUNC_H__
#define __OPENGLFUNC_H__

namespace OpenGLFunc
{
	unsigned int CreateFrameBuffer();
	unsigned int CreateTextureAttachment(int width, int height);
	unsigned int CreateDepthTextureAttachment(int width, int height);
	unsigned int CreateDepthBufferAttachment(int width, int height);
	void BindFrameBuffer(unsigned int frame_buffer, int width, int height);
	void UnbindCurrentFrameBuffer();
	void DeleteFrameBuffer(unsigned int frame_buffer);
	void DeleteRenderBuffer(unsigned int render_buffer);
	void DeleteTexture(unsigned int texture);
}

#endif // !__OPENGLFUNC_H__
