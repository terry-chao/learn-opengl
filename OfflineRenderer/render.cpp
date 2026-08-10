#include "render.h"

#include <MiniFB.h>

#include <iostream>

void render::run()
{
    mfb_window* window = mfb_open(m_title.c_str(), m_width, m_height);
    if (!window)
    {
        std::cerr << "Failed to open MiniFB window\n";
        return;
    }

    m_buffer.assign(static_cast<size_t>(m_width) * m_height, MFB_RGB(30, 30, 30));

    mfb_update_state state;
    do
    {
        state = mfb_update(window, m_buffer.data());
        if (state != MFB_STATE_OK)
        {
            break;
        }
    } while (mfb_wait_sync(window));

    mfb_close(window);
}
