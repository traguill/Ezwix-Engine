#include "Application.h"
#include "Material.h"
#include "Data.h"

Material::Material()
{
}

Material::~Material()
{
	for (std::vector<Uniform*>::iterator it = uniforms.begin(); it != uniforms.end(); ++it)
		delete *it;
}

void Material::AddUniform(const std::string& name, UniformType type, char* value)
{
	Uniform* uni = new Uniform();
	uni->name = name.data();
	uni->type = type;

	switch (type)
	{
	case U_BOOL:
		uni->value = new char[sizeof(bool)];
		memcpy(uni->value, value, sizeof(bool));
		break;
	case U_INT:
		uni->value = new char[sizeof(int)];
		memcpy(uni->value, value, sizeof(int));
		break;
	case U_FLOAT:
		break;
	case U_VEC2:
		break;
	case U_VEC3:
		break;
	case U_MAT4X4:
		break;
	case U_SAMPLER2D:
		break;

	}

	uniforms.push_back(uni);
}

bool Material::Save(const char * path) const
{
	//vertex_path(string) - fragment_path(string) - Uniforms(any type)
	unsigned int header[3] =
	{
		vertex_path.size(),
		fragment_path.size(),
		uniforms.size()
	};

	unsigned int size = sizeof(header) + sizeof(char) * header[0] + sizeof(char) * header[1];

	for (std::vector<Uniform*>::const_iterator uni = uniforms.begin(); uni != uniforms.end(); ++uni)
	{
		size += (*uni)->name.size() * sizeof(char); //name
		size += sizeof(int); //type
		switch ((*uni)->type)
		{
		case U_BOOL:
			size += sizeof(bool);
			break;
		case U_INT:
			size += sizeof(int);
			break;
		case U_FLOAT:
			size += sizeof(float);
			break;
		case U_VEC2:
			size += sizeof(float) * 2;
			break;
		case U_VEC3:
			size += sizeof(float) * 3;
			break;
		case U_MAT4X4:
			size += sizeof(float) * 16;
			break;
		case U_SAMPLER2D:
			//TODO: know the size of the string. Try first value as int with the size of the string?
			break;
		}
	}

	char* data = new char[size];
	char* cursor = data;
	unsigned int bytes;

	//Header
	bytes = sizeof(header);
	memcpy(cursor, header, bytes);
	cursor += bytes;

	//Vertex_path
	bytes = sizeof(char) * header[0];
	memcpy(cursor, vertex_path.c_str(), bytes);
	cursor += bytes;

	//Fragment_path
	bytes = sizeof(char) * header[1];
	memcpy(cursor, fragment_path.c_str(), bytes);
	cursor += bytes;

	for (std::vector<Uniform*>::const_iterator uni = uniforms.begin(); uni != uniforms.end(); ++uni)
	{
		bytes = (*uni)->name.size() * sizeof(char); //name
		memcpy(cursor, (*uni)->name.c_str(), bytes);
		cursor += bytes;

		bytes = sizeof(int); //type
		memcpy(cursor, &(*uni)->type, bytes);
		cursor += bytes;

		switch ((*uni)->type)
		{
		case U_BOOL:
			bytes= sizeof(bool);
			break;
		case U_INT:
			bytes= sizeof(int);
			break;
		case U_FLOAT:
			bytes= sizeof(float);
			break;
		case U_VEC2:
			bytes= sizeof(float) * 2;
			break;
		case U_VEC3:
			bytes= sizeof(float) * 3;
			break;
		case U_MAT4X4:
			bytes= sizeof(float) * 16;
			break;
		case U_SAMPLER2D:
			//TODO: know the size of the string. Try first value as int with the size of the string?
			break;
		}
		memcpy(cursor, (*uni)->value, bytes);
		cursor += bytes;
	}

	bool ret = App->file_system->Save(path, data, size);
	delete[] data;

	return ret;
}

void Material::Load(const char * path)
{
	char* buffer = nullptr;

	if (App->file_system->Load(path, &buffer) != 0)
	{
		char* cursor = buffer;

		uint header[3];
		uint bytes = sizeof(header);
		memcpy(header, cursor, bytes);
		cursor += bytes;

		vertex_path.resize(header[0]);
		memcpy(vertex_path._Myptr(), cursor, bytes);
	}

	if (buffer)
		delete[] buffer;
}
