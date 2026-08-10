#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct mfb_window;

class render
{
public:
    render() = default;
    ~render() = default;

    render(const render&) = delete;
    render& operator=(const render&) = delete;

    void run();

private:
    std::string m_title = "OfflineRenderer";
    uint32_t m_width = 800;
    uint32_t m_height = 600;
    std::vector<uint32_t> m_buffer;
};
