#include "Application.h"
#include "ShaderComplier.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") 

bool ShaderCompiler::TryCompileVertex(const char* path)
{
	char* buffer;
	int size = App->file_system->Load(path, &buffer);

	if (size <= 0)
	{
		delete[] buffer;
		LOG("Vertex shader: %s can't be loaded with filesystem.", path);
		return false;
	}

	string source = string(buffer, size);
	GLuint vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* gl_source = (const GLchar *)source.c_str();
	glShaderSource(vertex_shader, 1, &gl_source, 0);
	glCompileShader(vertex_shader);

	GLint success;
	GLchar info[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info);
		LOG("Vertex shader: %s compilation error (%s)", path, info);
	}

	glDeleteShader(vertex_shader);

	delete[] buffer;

	return (success != 0) ? true : false;
}

bool ShaderCompiler::TryCompileFragment(const char * path)
{
	char* buffer;
	int size = App->file_system->Load(path, &buffer);

	if (size <= 0)
	{
		delete[] buffer;
		LOG("Vertex shader: %s can't be loaded with filesystem.", path);
		return false;
	}

	string source = string(buffer, size);
	GLuint fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* gl_source = (const GLchar *)source.c_str();
	glShaderSource(fragment_shader, 1, &gl_source, 0);
	glCompileShader(fragment_shader);

	GLint success;
	GLchar info[512];
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info);
		LOG("Fragment shader: %s compilation error (%s)", path, info);
	}

	glDeleteShader(fragment_shader);

	delete[] buffer;

	return (success != 0) ? true : false;
}

bool ShaderCompiler::TryCompileShader(const char * vertex_path, const char * fragment_path)
{
	bool ret = false;

	int ret_vertex, ret_fragment;
	ret_vertex = CompileVertex(vertex_path);
	ret_fragment = CompileFragment(fragment_path);

	if (ret_vertex != -1 && ret_fragment != -1)
	{
		GLint success;
		GLuint vertex_shader = ret_vertex;
		GLuint fragment_shader = ret_fragment;

		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);

		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (success == 0)
		{
			GLchar info[512];
			glGetProgramInfoLog(shader_program, 512, NULL, info);
			LOG("Shader link error: %s", info);
		}
		else
			ret = true;

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		glDeleteShader(shader_program);
	}

	return ret;
}

int ShaderCompiler::CompileVertex(const char * path)
{
	int ret = -1;
	char* buffer;
	int size = App->file_system->Load(path, &buffer);

	if (size <= 0)
	{
		delete[] buffer;
		LOG("Vertex shader: %s can't be loaded with filesystem.", path);
		return ret;
	}

	string source = string(buffer, size);
	GLuint vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* gl_source = (const GLchar *)source.c_str();
	glShaderSource(vertex_shader, 1, &gl_source, 0);
	glCompileShader(vertex_shader);

	GLint success;
	GLchar info[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info);
		LOG("Vertex shader: %s compilation error (%s)", path, info);
	}
	else
		ret = vertex_shader;

	delete[] buffer;

	return ret;
}

int ShaderCompiler::CompileFragment(const char * path)
{
	int ret = -1;
	char* buffer;
	int size = App->file_system->Load(path, &buffer);

	if (size <= 0)
	{
		delete[] buffer;
		LOG("Vertex shader: %s can't be loaded with filesystem.", path);
		return ret;
	}

	string source = string(buffer, size);
	GLuint fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* gl_source = (const GLchar *)source.c_str();
	glShaderSource(fragment_shader, 1, &gl_source, 0);
	glCompileShader(fragment_shader);

	GLint success;
	GLchar info[512];
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info);
		LOG("Fragment shader: %s compilation error (%s)", path, info);
	}
	else
		ret = fragment_shader;
	delete[] buffer;

	return ret;
}

int ShaderCompiler::CompileShader(unsigned int vertex_id, unsigned int fragment_id)
{
	GLint success;
	GLuint vertex_shader = vertex_id;
	GLuint fragment_shader = fragment_id;

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		GLchar info[512];
		glGetProgramInfoLog(shader_program, 512, NULL, info);
		LOG("Shader link error: %s", info);
	}
	
	return shader_program;
}

int ShaderCompiler::LoadDefaultShader()
{
	GLuint vertex_shader, fragment_shader;
	GLuint shader = glCreateProgram();;

	const GLchar* vertex_code =
		"#version 330 core \n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec2 texCoord;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(position, 1.0f);\n"
		"	TexCoord = texCoord;\n"
		"}\n";

	const GLchar* fragment_code =
		"#version 330 core\n"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D _Texture;\n"
		"void main()\n"
		"{\n"
		"	color = texture(_Texture, TexCoord);\n"
		"}\n";

	
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_code, 0);
	glShaderSource(fragment_shader, 1, &fragment_code, 0);
	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	glAttachShader(shader, vertex_shader);
	glAttachShader(shader, fragment_shader);
	glLinkProgram(shader);

	return shader;
}

void ShaderCompiler::DeleteShader(unsigned int shader_id)
{
	glDeleteShader(shader_id);
}
