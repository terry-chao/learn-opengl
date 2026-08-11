#pragma once

#include "Camera.h"
#include "Common.h"
#include "SceneObject.h"

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

    std::shared_ptr<SceneObject> AddSceneObject(const Vector3f& position, const Vector3f& euler, float scale);

    std::string mTitle = "OfflineRenderer";
    int mViewportWidth = 800;
    int mViewportHeight = 600;

    Camera mCamera;
    std::vector<std::shared_ptr<SceneObject>> mSceneObjects;

    std::vector<uint32_t> mBuffer;
    std::atomic<int> mCurrentPixelIndex{0};

    std::atomic<bool> mRunning{false};
    std::thread mWorker;
};
