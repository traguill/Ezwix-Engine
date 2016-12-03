#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ResourceFileMesh.h"
#include "ComponentMesh.h"
#include "MeshImporter.h"
ResourceFileMesh::ResourceFileMesh(ResourceFileType type, const std::string& file_path, unsigned int uuid) : ResourceFile(type, file_path, uuid)
{
}

ResourceFileMesh::~ResourceFileMesh()
{
	delete mesh;
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
	App->renderer3D->RemoveBuffer(mesh->id_vertices);
	App->renderer3D->RemoveBuffer(mesh->id_indices);
	App->renderer3D->RemoveBuffer(mesh->id_uvs);
}
