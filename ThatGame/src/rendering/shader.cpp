#include "shader.h"
#include "../core/logging.h"
#include "../core/asserts.h"

#include <fstream>
#include <sstream>
#include <vector>

shader::shader(const std::string& vertex_filepath, const std::string& fragment_filepath)
{
    const std::string vertex_src = get_file_content(vertex_filepath);
    const std::string fragment_src = get_file_content(fragment_filepath);
    TG_ASSERT(!vertex_src.empty() && !fragment_src.empty(), "Failed to read shader files");

    const u32 vs = compile_shader(GL_VERTEX_SHADER, vertex_src);
    const u32 fs = compile_shader(GL_FRAGMENT_SHADER, fragment_src);
    TG_ASSERT(vs && fs, "Failed to compile shader sources");

    link(vs, fs);
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

std::string shader::get_file_content(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        LOG_ERROR("Failed to open shader file: {0}", filepath);
        return {};
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

u32 shader::compile_shader(GLenum type, const std::string& source)
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

void shader::link(u32 v_shader, u32 f_shader)
{
    m_id = glCreateProgram();
    glAttachShader(m_id, v_shader);
    glAttachShader(m_id, f_shader);
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

    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
}
