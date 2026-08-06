#include "shader.h"
#include "../core/logging.h"
#include "../core/asserts.h"

#include <vector>

u32 shader::compile(GLenum type, const std::string& source)
{
    const u32 id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    i32 success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        i32 length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> info_log(static_cast<size_t>(length));
        glGetShaderInfoLog(id, length, &length, info_log.data());
        LOG_ERROR("Shader compilation failed:\n{0}", info_log.data());
        glDeleteShader(id);
        return 0;
    }

    return id;
}

shader::shader(const std::string& vertex_src, const std::string& fragment_src)
{
    const u32 vs = compile(GL_VERTEX_SHADER, vertex_src);
    const u32 fs = compile(GL_FRAGMENT_SHADER, fragment_src);
    TG_ASSERT(vs && fs, "Failed to compile shader sources");

    m_id = glCreateProgram();
    glAttachShader(m_id, vs);
    glAttachShader(m_id, fs);
    glLinkProgram(m_id);

    i32 success = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        i32 length = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> info_log(static_cast<size_t>(length));
        glGetProgramInfoLog(m_id, length, &length, info_log.data());
        LOG_ERROR("Shader linking failed:\n{0}", info_log.data());
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

shader::~shader()
{
    delete_id();
}

void shader::bind()
{
    glUseProgram(m_id);
}

void shader::unbind()
{
    glUseProgram(0);
}

void shader::delete_id()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}
