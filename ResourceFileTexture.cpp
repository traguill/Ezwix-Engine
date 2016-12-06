#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ResourceFileTexture.h"
#include "MaterialImporter.h"

ResourceFileTexture::ResourceFileTexture(ResourceFileType type, const std::string& file_path, unsigned int uuid) : ResourceFile(type, file_path, uuid)
{
}

ResourceFileTexture::~ResourceFileTexture()
{
}

unsigned int ResourceFileTexture::GetTexture()
{
	return texture_id;
}

void ResourceFileTexture::SetProperties(unsigned int texture_id, unsigned int width, unsigned int height, unsigned int depth, unsigned int mips, unsigned int bytes)
{
	this->texture_id = texture_id;
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->mips = mips;
	this->bytes = bytes;
}

void ResourceFileTexture::LoadInMemory()
{
	MaterialImporter::Load(this);
}

void ResourceFileTexture::UnloadInMemory()
{
	App->renderer3D->RemoveBuffer(texture_id);
	App->resource_manager->RemoveResourceFromList(this);
}
