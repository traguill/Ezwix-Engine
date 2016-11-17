#include "Application.h"
#include "ModuleMeshes.h"

#include "Glew\include\glew.h"
#include <gl\GL.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

#include "ModuleGOManager.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "MeshImporter.h"

#include "MaterialImporter.h"

ModuleMeshes::ModuleMeshes(const char* name, bool start_enabled) : Module(name, start_enabled)
{}

ModuleMeshes::~ModuleMeshes()
{}

bool ModuleMeshes::Init(Data& config)
{
	return true;
}

bool ModuleMeshes::CleanUp()
{
	
	return true;
}

bool ModuleMeshes::Load(const char* path, const char* base_path)
{
	bool ret = false;
	char* buff;
	uint size = App->file_system->Load(path, &buff);

	if (size == 0)
	{
		LOG("Error loading %s", path);
		return ret; 
	}
	
	const aiScene* scene = aiImportFileFromMemory(buff, size, aiProcessPreset_TargetRealtime_MaxQuality, NULL);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* root = scene->mRootNode;

		//Load root childs. Do not load the root node (unnecessary)
		for (int i = 0; i < root->mNumChildren; i++)
		{
			LoadNode(root->mChildren[i], scene, NULL, base_path);
		}
		
		aiReleaseImport(scene);

		ret = true;
	}
	else
	{
		LOG("Error loading scene %s. ERROR: %s", path, aiGetErrorString());
	}

	delete[] buff;

	return ret;
}

uint ModuleMeshes::LoadTexture(const char* path)
{
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);

	return ilutGLBindTexImage();

}

void ModuleMeshes::LoadNode(aiNode* node,const aiScene* scene, GameObject* parent, const char* base_path)
{
	
}