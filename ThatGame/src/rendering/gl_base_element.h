#pragma once

#include <glad/glad.h>
#include "../core/defines.h"

class gl_base_element
{
public:
    virtual ~gl_base_element() = default;

    virtual void bind() {};
    virtual void unbind() {};
    virtual void delete_id() {};

    u32 get_id() const { return m_id; }

protected:
    u32 m_id = 0;
};
