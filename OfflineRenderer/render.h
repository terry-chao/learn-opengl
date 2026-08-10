#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class render
{
public:
    render() = default;
    ~render();

    render(const render&) = delete;
    render& operator=(const render&) = delete;

    void run();

private:
    void generate_colors();

    std::string m_title = "OfflineRenderer";
    uint32_t m_width = 800;
    uint32_t m_height = 600;

    std::vector<uint32_t> m_buffer;
    std::vector<uint32_t> m_back_buffer;
    std::mutex m_mutex;

    std::atomic<bool> m_running{false};
    std::thread m_worker;
};
