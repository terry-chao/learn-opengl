#pragma once

#include "application.h"

#include "../rendering/vertex_array.h"
#include "../rendering/gl_buffer.h"
#include "../rendering/shader.h"
#include "../rendering/render_commands.h"

#include <memory>

class sandbox_app : public application
{
protected:
    void on_init() override;
    void on_render() override;
    void on_shutdown() override;

private:
    std::shared_ptr<vertex_array> m_vao;
    std::shared_ptr<shader> m_shader;
};
