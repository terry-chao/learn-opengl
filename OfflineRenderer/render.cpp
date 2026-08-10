#include "render.h"

#include <MiniFB.h>

#include <chrono>
#include <cmath>
#include <iostream>

render::~render()
{
    m_running = false;
    if (m_worker.joinable())
    {
        m_worker.join();
    }
}

void render::generate_colors()
{
    float t = 0.0f;

    while (m_running.load(std::memory_order_relaxed))
    {
        const size_t pixel_count = static_cast<size_t>(m_width) * m_height;
        for (size_t i = 0; i < pixel_count; ++i)
        {
            const uint32_t x = static_cast<uint32_t>(i % m_width);
            const uint32_t y = static_cast<uint32_t>(i / m_width);

            const uint8_t r = static_cast<uint8_t>((x * 255u) / m_width);
            const uint8_t g = static_cast<uint8_t>((y * 255u) / m_height);
            const uint8_t b = static_cast<uint8_t>((std::sin(t + y * 0.01f) * 0.5f + 0.5f) * 255.0f);

            m_back_buffer[i] = MFB_RGB(r, g, b);
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_buffer.swap(m_back_buffer);
        }

        t += 0.08f;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void render::run()
{
    mfb_window* window = mfb_open(m_title.c_str(), m_width, m_height);
    if (!window)
    {
        std::cerr << "Failed to open MiniFB window\n";
        return;
    }

    const size_t pixel_count = static_cast<size_t>(m_width) * m_height;
    m_buffer.assign(pixel_count, MFB_RGB(30, 30, 30));
    m_back_buffer.assign(pixel_count, MFB_RGB(30, 30, 30));

    m_running = true;
    m_worker = std::thread(&render::generate_colors, this);

    std::vector<uint32_t> display;
    display.resize(pixel_count);

    mfb_update_state state;
    do
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            display = m_buffer;
        }

        state = mfb_update(window, display.data());
        if (state != MFB_STATE_OK)
        {
            break;
        }
    } while (mfb_wait_sync(window));

    m_running = false;
    if (m_worker.joinable())
    {
        m_worker.join();
    }

    mfb_close(window);
}
