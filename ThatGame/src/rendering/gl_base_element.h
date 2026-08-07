#pragma once

#include "../core/defines.h"

class gl_base_element
{
public:
    virtual ~gl_base_element() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual void delete_id() = 0;

    u32 get_id() const { return m_id; }

protected:
    u32 m_id = 0;
};
