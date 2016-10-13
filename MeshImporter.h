#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

struct aiMesh;
struct Mesh;

#include <string>

namespace MeshImporter
{
	bool Import(const aiMesh* mesh, std::string& output);
	bool Save(Mesh& mesh, std::string& output_file);

	Mesh* Load(const char* path);

}

#endif // !__MESH_IMPORTER_H_
