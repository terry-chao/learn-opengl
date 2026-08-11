#pragma once

#include "Camera.h"
#include "Common.h"
#include "Primitive.h"
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

    Renderer() = default;
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void Run();

private:
    void RunRenderThread();

    Color RenderPixel(int x, int y);
    Color RenderSubPixel(float x, float y);

    SceneObject* AddSceneObject(const Vector3f& position, const Vector3f& euler, float scale);

    std::string mTitle = "OfflineRenderer";
    int mViewportWidth = 800;
    int mViewportHeight = 600;

    Camera mCamera;
    std::vector<std::unique_ptr<SceneObject>> mSceneObjects;
    std::vector<std::unique_ptr<Primitive>> mPrimitives;

    std::vector<uint32_t> mBuffer;
    std::atomic<int> mCurrentPixelIndex{0};

    std::atomic<bool> mRunning{false};
    std::thread mWorker;
};
