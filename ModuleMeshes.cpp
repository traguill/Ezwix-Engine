#include "Application.h"
#include "ModuleMeshes.h"

#include "Glew\include\glew.h"
#include <gl\GL.h>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleMeshes::ModuleMeshes(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleMeshes::~ModuleMeshes()
{}

bool ModuleMeshes::Init()
{
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
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

			//Vertices
			mesh.num_vertices = mesh_to_load->mNumVertices;
			mesh.vertices = new float[mesh.num_vertices * 3];
			memcpy(mesh.vertices, mesh_to_load->mVertices, sizeof(float)*mesh.num_vertices * 3);
			LOG("Mesh Loaded with %d vertices", mesh.num_vertices);

			//Load buffer to VRAM
			glGenBuffers(1, (GLuint*)&(mesh.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

			//Indices
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
			//Load buffer to VRAM
			glGenBuffers(1, (GLuint*) &(mesh.id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

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