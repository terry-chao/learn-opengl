#pragma once

#include <string>

#include "gl_base_element.h"
#include <glm/glm.hpp>

class shader : public gl_base_element
{
public:
    shader() = default;
    shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
    ~shader() override;

    void bind() override;
    void unbind() override;
    void delete_id() override;

    void set_mat4(const std::string& name, const glm::mat4& value);
    void set_mat3(const std::string& name, const glm::mat3& value);
    void set_mat2(const std::string& name, const glm::mat2& value);
    void set_vec4(const std::string& name, const glm::vec4& value);
    void set_vec3(const std::string& name, const glm::vec3& value);
    void set_vec2(const std::string& name, const glm::vec2& value);
    void set_int(const std::string& name, i32 value);
    void set_float(const std::string& name, f32 value);

private:
    std::string get_file_content(const std::string& filepath);
    static u32 compile_shader(GLenum type, const std::string& source);
    void link(u32 v_shader, u32 f_shader);
    i32 get_uniform_location(const std::string& name) const;
};
