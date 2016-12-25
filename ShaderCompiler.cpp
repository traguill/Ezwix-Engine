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
