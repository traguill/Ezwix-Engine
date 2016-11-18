#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include <string>

struct aiMesh;
struct aiNode;
struct aiScene;
struct Mesh;

namespace MeshImporter
{
	//File is the final path of the imported file with the extension. Path is from the file is converted. Base path is the directory to save all generated files
	bool Import(const char* file, const char* path,const char* base_path);
	void ImportNode(aiNode* node, const aiScene* scene, GameObject* parent, const char* base_path, vector<GameObject*>& objects_created, string folder_path);
	bool ImportMesh(const aiMesh* mesh, const char* folder_path, string& output_name);
	bool Save(Mesh& mesh, const char* folder_path, string& output_name);
	Mesh* Load(const char* path);
}

#endif // !__MESH_IMPORTER_H_
