#include "ResourceFileMesh.h"
#include "ComponentMesh.h"
#include "MeshImporter.h"
ResourceFileMesh::ResourceFileMesh(ResourceFileType type, const std::string& file_path, unsigned int uuid) : ResourceFile(type, file_path, uuid)
{
}

ResourceFileMesh::~ResourceFileMesh()
{
	mesh = nullptr;
}

Mesh * ResourceFileMesh::GetMesh() const
{
	return mesh;
}

void ResourceFileMesh::LoadInMemory()
{
	mesh = MeshImporter::Load(file_path.data());
}

void ResourceFileMesh::UnloadInMemory()
{
	//TODO: Unload a mesh
}
