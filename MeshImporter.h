#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

struct aiMesh;
struct Mesh;

#include <string>

namespace MeshImporter
{
	//File is the final path of the imported file with the extension. Path is from the file is converted. Base path is the directory to save all generated files
	bool Import(const char* file, const char* path,const char* base_path);
	void ImportNode(aiNode* node, const aiScene* scene, GameObject* parent, const char* base_path, Data& conf, vector<GameObject*>& objects_created);
	bool ImportMesh(const aiMesh* mesh, std::string& output);
	bool Save(Mesh& mesh, std::string& output_file);
	Mesh* Load(const char* path);
}

#endif // !__MESH_IMPORTER_H_
