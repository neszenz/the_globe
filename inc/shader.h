// based on 'https://github.com/HectorPeeters/opengl_premake_boilerplate'
#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const char *name, const char *vertexPath, const char *fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void Uniform1f(std::string name, float value) const;
    void Uniform2f(std::string name, glm::vec2 &value) const;
    void Uniform3f(std::string name, glm::vec3 &value) const;
    void Uniform4f(std::string name, glm::vec4 &value) const;

    void Uniform1i(std::string name, int value) const;
    void Uniform2i(std::string name, glm::ivec2 &value) const;
    void Uniform3i(std::string name, glm::ivec3 &value) const;
    void Uniform4i(std::string name, glm::ivec4 &value) const;

    void UniformMat4(std::string name, glm::mat4 &value) const;

private:
    int m_ProgramId;

    std::map<std::string, int> m_Uniforms;

    std::string m_Name;
    const char *m_VertexPath, *m_FragmentPath;
};
