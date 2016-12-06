#ifndef __RESOURCEFILETEXTURE_H__
#define __RESOURCEFILETEXTURE_H__

#include "ResourceFile.h"

class ResourceFileTexture : public ResourceFile
{
public:
	ResourceFileTexture(ResourceFileType type, const std::string& file_path, unsigned int uuid);
	~ResourceFileTexture();

	unsigned int GetTexture();
	void SetProperties(unsigned int texture_id, unsigned int width, unsigned int height, unsigned int depth, unsigned int mips, unsigned int bytes);

private:

	void LoadInMemory();
	void UnloadInMemory();

private:
	unsigned int texture_id = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;
	unsigned int mips = 0;
};

#endif // !__RESOURCEFILETEXTURE_H__
