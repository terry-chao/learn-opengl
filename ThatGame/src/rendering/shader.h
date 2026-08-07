#pragma once

#include "gl_base_element.h"

#include <glad/glad.h>
#include <string>

class shader : public gl_base_element
{
public:
    shader() = default;
    shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
    ~shader() override;

    void bind() override;
    void unbind() override;
    void delete_id() override;

private:
    std::string get_file_content(const std::string& filepath);
    static u32 compile_shader(GLenum type, const std::string& source);
    void link(u32 v_shader, u32 f_shader);
};
