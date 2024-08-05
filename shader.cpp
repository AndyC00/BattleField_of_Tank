// This include: 
#include "shader.h"

// Local includes: 
#include "logmanager.h" 
#include "matrix4.h"

// Library includes: 
#include <cassert> 
#include <fstream> 
#include <sstream> 
#include <string>

Shader::Shader()
	: m_vertexShader(0)
	, m_pixelShader(0)
	, m_shaderProgram(0)
{

}

Shader::~Shader()
{
	Unload();
}
bool
Shader::Load(const char* vertexFile, const char* pixelFile)
{
	bool vertexCompiled = CompileShader(vertexFile, GL_VERTEX_SHADER, m_vertexShader); 
	bool pixelCompiled = CompileShader(pixelFile, GL_FRAGMENT_SHADER, m_pixelShader);

	if (vertexCompiled == false || pixelCompiled == false)
	{
		LogManager::GetInstance().Log("Shaders failed to compile!"); 
		assert(0);
		return false;
	}
	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_pixelShader);
	glLinkProgram(m_shaderProgram);
	return IsValidProgram();
}
void Shader::Unload()
{
	glDeleteProgram(m_shaderProgram);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_pixelShader);
}
void Shader::SetActive()
{
	assert(m_shaderProgram);
	glUseProgram(m_shaderProgram);
}

void
Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	GLuint location = glGetUniformLocation(m_shaderProgram, name);
	glUniformMatrix4fv(location, 1, GL_TRUE, (float*)&matrix);
}

void
Shader::SetVector4Uniform(const char* name, float x, float y, float z, float w)
{
	GLuint location = glGetUniformLocation(m_shaderProgram, name);

	float vec4[4];

	vec4[0] = x; vec4[1] = y; vec4[2] = z; vec4[3] = w;

	glUniform4fv(location, 1, vec4);
}

	bool
		Shader::CompileShader(const char* filename, GLenum shaderType, GLuint& outShader)
	{
		std::ifstream shaderFile(filename);
		if (shaderFile.is_open())
		{
			std::stringstream sstream; sstream << shaderFile.rdbuf();
			std::string shaderCode = sstream.str();
			const char* pShaderCode = shaderCode.c_str();
			outShader = glCreateShader(shaderType);
			glShaderSource(outShader, 1, &(pShaderCode), 0);
			glCompileShader(outShader);

			if (!IsCompiled(outShader))
			{
				LogManager::GetInstance().Log("Shader failed to compile!");
				return false;
			}
		}
		else
		{
			LogManager::GetInstance().Log("Shader file not found!");
			return false;
		}

		return true;
	}
	bool
		Shader::IsCompiled(GLuint shader)
	{
		GLint compiledStatus;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledStatus);

		if (compiledStatus != GL_TRUE)
		{
			char error[1024]; error[0] = 0;
			glGetShaderInfoLog(shader, 1023, 0, error);
			LogManager::GetInstance().Log("Shader failed to compile!");
			return false;
		}

	return true;
	}

	bool Shader::IsValidProgram()
	{
		GLint linkedStatus;
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &linkedStatus);
		if (linkedStatus != GL_TRUE)
		{
			char error[1024];
			error[0] = 0;
			glGetShaderInfoLog(m_shaderProgram, 1023, 0, error);

			LogManager::GetInstance().Log("Shader failed to link!");
			assert(0);

			return false;
		}

		return true;
	}
