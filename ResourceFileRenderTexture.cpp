#include "Application.h"
#include "ResourceFileRenderTexture.h"
#include "OpenGLFunc.h"

ResourceFileRenderTexture::ResourceFileRenderTexture(ResourceFileType type, const std::string& file_path, unsigned int uuid) : ResourceFile(type, file_path, uuid)
{
}

ResourceFileRenderTexture::~ResourceFileRenderTexture()
{
}

void ResourceFileRenderTexture::Bind()
{
	OpenGLFunc::BindFrameBuffer(frame_buffer, width, height);
}

void ResourceFileRenderTexture::Unbind()
{
	OpenGLFunc::UnbindCurrentFrameBuffer();
}

void ResourceFileRenderTexture::LoadInMemory()
{
	char* buffer;
	int size = App->file_system->Load(file_path.data(), &buffer);

	if (size > 0)
	{
		Data data(buffer);
		width = data.GetInt("width");
		height = data.GetInt("height");
		use_depth_as_texture = data.GetBool("use_depth_as_texture");

		frame_buffer = OpenGLFunc::CreateFrameBuffer();
		texture_buffer = OpenGLFunc::CreateTextureAttachment(width, height);

		if (use_depth_as_texture)
			depth_buffer = OpenGLFunc::CreateDepthTextureAttachment(width, height);
		else
			depth_buffer = OpenGLFunc::CreateDepthBufferAttachment(width, height);

		OpenGLFunc::UnbindCurrentFrameBuffer();
	}
	else
		LOG("Could not load resource %s", file_path.data());


	if(buffer)
		delete[] buffer;
}

void ResourceFileRenderTexture::UnloadInMemory()
{
	OpenGLFunc::DeleteFrameBuffer(frame_buffer);
	OpenGLFunc::DeleteTexture(texture_buffer);
	if (use_depth_as_texture)
		OpenGLFunc::DeleteTexture(depth_buffer);
	else
		OpenGLFunc::DeleteRenderBuffer(depth_buffer);
}
