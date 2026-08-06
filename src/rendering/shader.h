#pragma once

#include "gl_base_element.h"

#include <glad/glad.h>
#include <string>

class shader : public gl_base_element
{
public:
    shader(const std::string& vertex_src, const std::string& fragment_src);
    ~shader() override;

    void bind() override;
    void unbind() override;
    void delete_id() override;

private:
    static u32 compile(GLenum type, const std::string& source);
};
