#ifndef   SHADER_H 
#define   SHADER_H 

#include "glew.h"

// Forward declarations: 
struct Matrix4;
class Shader
{
// Member methods: 
public:
	Shader();
	~Shader();
	bool Load(const char* vertexFile, const char* pixelFile); 
	void Unload();

	void SetActive();

	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetVector4Uniform(const char* name, float x, float y, float z, float w);

protected:

private:
	bool IsValidProgram();

	static bool CompileShader(const char* filename, GLenum shaderType, GLuint& outShader);
	static bool IsCompiled(GLuint shader);

// Member data: 
public:

protected:
	GLuint m_vertexShader; 
	GLuint m_pixelShader; 
	GLuint m_shaderProgram;

private:

};

#endif //   SHADER_H 
