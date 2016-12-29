#ifndef __RESOURCEFILEMATERIAL_H__
#define __RESOURCEFILEMATERIAL_H__

#include "ResourceFile.h"

class ResourceFileMaterial : public ResourceFile
{
public:
	ResourceFileMaterial(ResourceFileType type, const std::string& file_path, unsigned int uuid);
	~ResourceFileMaterial();

	uint GetShaderId();

private:

	void LoadInMemory();
	void UnloadInMemory();

public:
	Material material;
private:
	uint shader_id;
	uint vertex_id;
	uint fragment_id;

};

#endif // !__RESOURCEFILEMATERIAL_H__