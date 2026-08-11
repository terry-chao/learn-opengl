#pragma once

#include "Common.h"
#include "Scene.h"

#include <atomic>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

class Renderer
{
public:
    static constexpr int SamplePerPixel = 4;

    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void Run();

private:
    void RunRenderThread();
    void JoinWorkers();

    Color RenderPixel(int x, int y);
    Color RenderSubPixel(float x, float y);

    std::string mTitle = "OfflineRenderer";
    int mViewportWidth = 800;
    int mViewportHeight = 600;

    Scene mScene;

    std::vector<uint32_t> mBuffer;
    std::atomic<int> mCurrentPixelIndex{0};

    std::atomic<bool> mRunning{false};
    std::vector<std::thread> mWorkers;
};
