// based on 'https://github.com/HectorPeeters/opengl_premake_boilerplate'
#include "shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <streambuf>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdexcept>

#include "gl_utils.h"

unsigned int CreateShader(const char *name, const char *source, unsigned int type)
{
    unsigned int id;

    GL(id = glCreateShader(type));
    GL(glShaderSource(id, 1, &source, NULL));
    GL(glCompileShader(id));

    char infoLog[512];

    int success;
    GL(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GL(glGetShaderInfoLog(id, 512, NULL, infoLog));
        fprintf(stderr, "Shader Error: Failed to compile shader '%s': \n%s\n", name, infoLog);
        throw std::runtime_error("CreateShader() error");
    }

    printf("Loaded %s shader '%s'\n", type == 35633 ? "vertex" : "fragment", name);

    return id;
}

Shader::Shader(const char *name, const char *vertexPath, const char *fragmentPath)
    : m_Name(name), m_VertexPath(vertexPath), m_FragmentPath(fragmentPath)
{
    std::string vertexSource;
    std::string fragmentSource;
    std::ifstream vertexStream;
    std::ifstream fragmentStream;

    vertexStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexStream.open(m_VertexPath);
        fragmentStream.open(m_FragmentPath);
        std::stringstream vertexStringStream, fragmentStringStream;

        vertexStringStream << vertexStream.rdbuf();
        fragmentStringStream << fragmentStream.rdbuf();

        vertexStream.close();
        fragmentStream.close();

        vertexSource = vertexStringStream.str();
        fragmentSource = fragmentStringStream.str();
    }
    catch (std::ifstream::failure e)
    {
        fprintf(stderr, "Shader Error: Failed to load shader '%s'\n", m_Name.c_str());
        throw std::runtime_error("Shader() error");
    }

    GL(m_ProgramId = glCreateProgram());

    unsigned int vertexId = CreateShader(m_Name.c_str(), vertexSource.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentId = CreateShader(m_Name.c_str(), fragmentSource.c_str(), GL_FRAGMENT_SHADER);

    GL(glAttachShader(m_ProgramId, vertexId));
    GL(glAttachShader(m_ProgramId, fragmentId));
    GL(glLinkProgram(m_ProgramId));

    int success;
    char infoLog[512];

    GL(glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &success));
    if (!success)
    {
        GL(glGetShaderInfoLog(m_ProgramId, 512, NULL, infoLog));
        fprintf(stderr, "Shader Error: Failed to link shader program '%s': \n%s\n", m_Name.c_str(), infoLog);
        throw std::runtime_error("Shader() error");
    }

    GL(glDeleteShader(vertexId));
    GL(glDeleteShader(fragmentId));

    int uniformCount;
    GL(glGetProgramiv(m_ProgramId, GL_ACTIVE_UNIFORMS, &uniformCount));

    int length, size;
    GLenum type;
    for (int i = 0; i < uniformCount; i++)
    {
        char name[64];
        GL(glGetActiveUniform(m_ProgramId, (GLuint)i, 64, &length, &size, &type, name));

        GL(unsigned int uniformLocation = glGetUniformLocation(m_ProgramId, name));

        m_Uniforms.insert(std::pair<const char *, unsigned int>(name, uniformLocation));
    }

    printf("Loaded shader program '%s', uniforms: %d\n", m_Name.c_str(), uniformCount);
}

Shader::~Shader()
{
    Unbind();
    GL(glDeleteProgram(m_ProgramId));
}

void Shader::Bind()
{
    GL(glUseProgram(m_ProgramId));
}

void Shader::Unbind()
{
    GL(glUseProgram(0));
}

void Shader::Uniform1f(std::string name, float value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniform1f(location, value));
}

void Shader::Uniform2f(std::string name, glm::vec2 &value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniform2f(location, value.x, value.y));
}

void Shader::Uniform3f(std::string name, glm::vec3 &value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniform3f(location, value.x, value.y, value.z));
}

void Shader::Uniform4f(std::string name, glm::vec4 &value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniform4f(location, value.x, value.y, value.z, value.w));
}

void Shader::Uniform1i(std::string name, int value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniform1i(location, value));
}

void Shader::Uniform2i(std::string name, glm::ivec2 &value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniform2i(location, value.x, value.y));
}

void Shader::Uniform3i(std::string name, glm::ivec3 &value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniform3i(location, value.x, value.y, value.z));
}

void Shader::Uniform4i(std::string name, glm::ivec4 &value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniform4i(location, value.x, value.y, value.z, value.w));
}

void Shader::UniformMat4(std::string name, glm::mat4 &value)
{
    unsigned int location = m_Uniforms[name];
    GL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}
