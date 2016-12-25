#ifndef __SHADER_COMPILER_H__
#define __SHADER_COMPILER_H__

namespace ShaderCompiler
{
	bool TryCompileVertex(const char* file_path);
	bool TryCompileFragment(const char* file_path);
	bool TryCompileShader(const char* vertex_path, const char* fragment_path);

	int CompileVertex(const char* file_path);
	int CompileFragment(const char* file_path);
}

#endif // !__SHADER_COMPILER_H__
