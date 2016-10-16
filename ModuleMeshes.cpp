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

ModuleMeshes::ModuleMeshes(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleMeshes::~ModuleMeshes()
{}

bool ModuleMeshes::Init()
{
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	//Initialize Devil
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	return true;
}

bool ModuleMeshes::CleanUp()
{
	aiDetachAllLogStreams();
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
	//Transformation ------------------------------------------------------------------------------------------------------------------
	GameObject* go_root = App->go_manager->CreateGameObject(parent);

	ComponentTransform* c_transform = (ComponentTransform*)go_root->AddComponent(C_TRANSFORM);

	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);

	math::float3 pos;
	pos.x = translation.x; pos.y = translation.y; pos.z = translation.z;
	c_transform->SetPosition(pos);

	math::Quat rot;
	rot.x = rotation.x; rot.y = rotation.y; rot.z = rotation.z; rot.w = rotation.w;
	c_transform->SetRotation(rot);

	math::float3 scale;
	scale.x = scaling.x; scale.y = scaling.y; scale.z = scaling.z;
	c_transform->SetScale(scale);

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh_to_load = scene->mMeshes[node->mMeshes[i]];

		GameObject* game_object; 

		//Transform
		if (node->mNumMeshes > 1)
		{
			game_object= App->go_manager->CreateGameObject(go_root);
			game_object->AddComponent(C_TRANSFORM);
		}
		else
		{
			game_object = go_root;
		}

		if(node->mName.length > 0)
			game_object->name = node->mName.C_Str();
		

		//Mesh --------------------------------------------------------------------------------------------------------------------------------

		ComponentMesh* c_mesh = (ComponentMesh*)game_object->AddComponent(C_MESH);

		string mesh_path;
		bool ret = MeshImporter::Import(mesh_to_load, mesh_path);

		Mesh* mesh = MeshImporter::Load(mesh_path.data());

	
		//Vertices ------------------------------------------------------------------------------------------------------

		//Load buffer to VRAM
		glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->vertices, GL_STATIC_DRAW);

		//Indices --------------------------------------------------------------------------------------------------------

		//Load indices buffer to VRAM
		glGenBuffers(1, (GLuint*) &(mesh->id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

		//Load UVs -----------------------------------------------------------------------------------------------------------------------

		glGenBuffers(1, (GLuint*)&(mesh->id_uvs));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->num_uvs, mesh->uvs, GL_STATIC_DRAW);
		

		c_mesh->SetMesh(mesh);

		//Load Textures --------------------------------------------------------------------------------------------------------------------
		
		aiMaterial* material = scene->mMaterials[mesh_to_load->mMaterialIndex];
		//uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		if (material)
		{
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			if (path.length > 0)
			{
				ComponentMaterial* c_material = (ComponentMaterial*)game_object->AddComponent(C_MATERIAL);
				
				string texture_name;

				string complete_path = base_path;
				complete_path += path.data;
				
				complete_path.erase(0, 1);

				MaterialImporter::Import("texture", complete_path.data(), texture_name);
				c_material->texture_id = LoadTexture(texture_name.data());

				LOG("Texture id %i Load: %s", c_material->texture_id, texture_name.data());
			}
			
		}
		
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, go_root, base_path);
	}
}