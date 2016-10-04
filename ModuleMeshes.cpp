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

vector<Mesh> ModuleMeshes::Load(const char* path)
{
	vector<Mesh> ret;
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
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh_to_load = scene->mMeshes[i];
			Mesh mesh;

			//Vertices ------------------------------------------------------------------------------------------------------
			mesh.num_vertices = mesh_to_load->mNumVertices;
			mesh.vertices = new float[mesh.num_vertices * 3];
			memcpy(mesh.vertices, mesh_to_load->mVertices, sizeof(float)*mesh.num_vertices * 3);
			LOG("Mesh Loaded with %d vertices", mesh.num_vertices);

			//Load buffer to VRAM
			glGenBuffers(1, (GLuint*)&(mesh.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

			//Indices --------------------------------------------------------------------------------------------------------
			if (mesh_to_load->HasFaces())
			{
				mesh.num_indices = mesh_to_load->mNumFaces * 3;
				mesh.indices = new uint[mesh.num_indices]; 
				for (uint f = 0; f < mesh_to_load->mNumFaces; f++)
				{
					if (mesh_to_load->mFaces[f].mNumIndices != 3)
					{
						LOG("WARNING: geometry with face != 3 indices is trying to be loaded");
					}
					else
					{
						memcpy(&mesh.indices[f * 3], mesh_to_load->mFaces[f].mIndices, 3 * sizeof(uint));
					}			
				}
			}

			//Load indices buffer to VRAM
			glGenBuffers(1, (GLuint*) &(mesh.id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

			//Load UVs -----------------------------------------------------------------------------------------------------------------------
			if (mesh_to_load->HasTextureCoords(0))
			{
				mesh.num_uvs = mesh_to_load->mNumVertices; //Same size as vertices
				mesh.uvs = new float[mesh.num_uvs * 2];
				for (int uvs_item = 0; uvs_item < mesh.num_uvs; uvs_item++)
				{
					memcpy(&mesh.uvs[uvs_item * 2], &mesh_to_load->mTextureCoords[0][uvs_item].x, sizeof(float));
					memcpy(&mesh.uvs[(uvs_item*2)+1], &mesh_to_load->mTextureCoords[0][uvs_item].y, sizeof(float));
				}

				glGenBuffers(1, (GLuint*)&(mesh.id_uvs));
				glBindBuffer(GL_ARRAY_BUFFER, mesh.id_uvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2 * mesh.num_uvs, mesh.uvs, GL_STATIC_DRAW);
			}

			//Load Textures --------------------------------------------------------------------------------------------------------------------
			/*const aiMaterial* material = scene->mMaterials[i];
			
			if (material != nullptr)
			{
				aiString texture_path;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS) //For now only first diffuse texture
				{
					char* texture_buffer;
					if (App->file_system->Load(texture_path.data, &texture_buffer) != 0)
					{
						LOG("Texture load correctly");
					}

				}
				else
				{
					LOG("Error when loading texture of mesh %s", mesh_to_load->mName.data);
				}
			}*/
			

			ret.push_back(mesh);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s. ERROR: %s", path, aiGetErrorString());
	}

	delete[] buff;

	return ret;
}

uint ModuleMeshes::LoadTexture(char* path)
{
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);

	return ilutGLBindTexImage();

}