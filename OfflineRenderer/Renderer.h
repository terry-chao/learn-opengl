#pragma once

#include "Camera.h"
#include "Common.h"
#include "Disk.h"

#include <atomic>
#include <cstdint>
#include <memory>
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

    Color RenderPixel(int x, int y);
    Color RenderSubPixel(float x, float y);

    std::string mTitle = "OfflineRenderer";
    int mViewportWidth = 800;
    int mViewportHeight = 600;

    Camera mCamera;
    std::unique_ptr<Primitive> mDisk;

    std::vector<uint32_t> mBuffer;
    std::atomic<int> mCurrentPixelIndex{0};

    std::atomic<bool> mRunning{false};
    std::thread mWorker;
};
