#include "Application.h"
#include "ModuleFileSystem.h"
#include "MeshImporter.h"
#include "Globals.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "ModuleMeshes.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

bool MeshImporter::Import(const aiMesh * mesh_to_load, std::string & output)
{
	//Mesh --------------------------------------------------------------------------------------------------------------------------------

	Mesh mesh;
	
	//Vertices ------------------------------------------------------------------------------------------------------
	mesh.num_vertices = mesh_to_load->mNumVertices;
	mesh.vertices = new float[mesh.num_vertices * 3];
	memcpy(mesh.vertices, mesh_to_load->mVertices, sizeof(float)*mesh.num_vertices * 3);
	

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

	//Load UVs -----------------------------------------------------------------------------------------------------------------------
	if (mesh_to_load->HasTextureCoords(0))
	{
		mesh.num_uvs = mesh_to_load->mNumVertices; //Same size as vertices
		mesh.uvs = new float[mesh.num_uvs * 2];
		for (int uvs_item = 0; uvs_item < mesh.num_uvs; uvs_item++)
		{
			memcpy(&mesh.uvs[uvs_item * 2], &mesh_to_load->mTextureCoords[0][uvs_item].x, sizeof(float));
			memcpy(&mesh.uvs[(uvs_item * 2) + 1], &mesh_to_load->mTextureCoords[0][uvs_item].y, sizeof(float));
		}
	}

	//Normals ---------------------------------------------------------------------------------------------------------
	if (mesh_to_load->HasNormals())
	{
		mesh.normals = new float[mesh.num_vertices * 3];
		memcpy(mesh.normals, mesh_to_load->mNormals, sizeof(float) * mesh.num_vertices * 3);
	}

	//Colors --------------------------------------------------------------------------------------------------------
	if (mesh_to_load->HasVertexColors(0))
	{
		mesh.colors = new float[mesh.num_vertices * 3];
		memcpy(mesh.colors, mesh_to_load->mColors, sizeof(float) * mesh.num_vertices * 3);
	}
	
	mesh.name = (mesh_to_load->mName.length != 0) ? mesh_to_load->mName.C_Str() : "mesh";
	
	return Save(mesh, output);
}

bool MeshImporter::Save(Mesh& mesh, std::string& output_file)
{
	bool ret = false;

	//Indices - Vertices - Normals - Colors - UVs
	uint header[5] =
	{
		mesh.num_indices,
		mesh.num_vertices,
		(mesh.normals) ? mesh.num_vertices : 0,
		(mesh.colors) ? mesh.num_vertices : 0,
		mesh.num_uvs
	};

	uint size = sizeof(header) + sizeof(uint) * header[0] + sizeof(float) * header[1] * 3;
	if (header[2] != 0) size += sizeof(float) * header[2] * 3;
	if (header[3] != 0) size += sizeof(float) * header[3] * 3;
	size += sizeof(float) * header[4] * 2;

	char* data = new char[size];
	char* cursor = data;

	//Header
	uint bytes = sizeof(header);
	memcpy(cursor, header, bytes);

	cursor += bytes;

	//Indices
	bytes = sizeof(uint) * header[0];
	memcpy(cursor, mesh.indices, bytes);

	cursor += bytes;

	//Vertices
	bytes = sizeof(float) * header[1] * 3;
	memcpy(cursor, mesh.vertices, bytes);

	cursor += bytes;

	//Note: don't update bytes because for normals and colors is the same size as vertices

	//Normals
	if (header[2] != 0)
	{
		memcpy(cursor, mesh.normals, bytes);
		cursor += bytes;
	}

	//Colors
	if (header[3] != 0)
	{
		memcpy(cursor, mesh.colors, bytes);
		cursor += bytes;
	}

	//Uvs
	bytes = sizeof(float) * header[4] * 2;
	memcpy(cursor, mesh.uvs, bytes);

	ret = App->file_system->SaveUnique(mesh.name, data, size, LIBRARY_MESHES_FOLDER, "ezx", output_file);

	delete[] data;
	data = nullptr;

	return ret;
}

Mesh * MeshImporter::Load(const char * path)
{
	Mesh* mesh = nullptr;
	char* buffer;
	
	if (App->file_system->Load(path, &buffer) != 0)
	{
		mesh = new Mesh();

		char* cursor = buffer;

		uint header[5];
		uint bytes = sizeof(header);
		memcpy(header, cursor, bytes);

		mesh->num_indices = header[0];
		mesh->num_vertices = header[1];
		mesh->num_uvs = header[4];

		//Indices
		cursor += bytes;
		bytes = sizeof(uint) * mesh->num_indices;
		mesh->indices = new uint[mesh->num_indices];
		memcpy(mesh->indices, cursor, bytes);

		//Vertices
		cursor += bytes;
		bytes = sizeof(float) * mesh->num_vertices * 3;
		mesh->vertices = new float[mesh->num_vertices * 3];
		memcpy(mesh->vertices, cursor, bytes);

		//Normals
		cursor += bytes;
		if (header[2] != 0)
		{
			bytes = sizeof(float) * mesh->num_vertices * 3;
			mesh->normals = new float[mesh->num_vertices * 3];
			memcpy(mesh->normals, cursor, bytes);

			cursor += bytes;
		}

		//Colors
		if (header[3] != 0)
		{
			bytes = sizeof(float) * mesh->num_vertices * 3;
			mesh->colors = new float[mesh->num_vertices * 3];
			memcpy(mesh->colors, cursor, bytes);

			cursor += bytes;
		}

		//Uvs
		bytes = sizeof(float) * mesh->num_uvs * 2;
		mesh->uvs = new float[mesh->num_uvs * 2];
		memcpy(mesh->uvs, cursor, bytes);
	}

	delete[] buffer;
	buffer = nullptr;

	return mesh;
}
