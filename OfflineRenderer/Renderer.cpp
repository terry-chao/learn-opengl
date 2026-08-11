#include "Renderer.h"

#include <MiniFB.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <random>

Renderer::Renderer()
{
    Scene* scene = Scene::LoadFromXML("assets/scene02.xml");
    if (!scene)
    {
        std::cerr << "Failed to load assets/scene02.xml, using empty scene\n";
        return;
    }

    mScene = std::move(*scene);
    delete scene;
}

Renderer::~Renderer()
{
    mRunning = false;
    JoinWorkers();
}

void Renderer::JoinWorkers()
{
    for (std::thread& worker : mWorkers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
    mWorkers.clear();
}

Color Renderer::RenderPixel(int x, int y)
{
    // 每线程独立 RNG，避免 glm::linearRand 全局状态在多线程下竞争
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    const int N = SamplePerPixel;
    Color resultColor(0, 0, 0);
    for (int i = 0; i < N; ++i)
    {
        float px = x + dist(rng);
        float py = y + dist(rng);
        resultColor += RenderSubPixel(px, py) / static_cast<float>(N);
    }
    return resultColor;
}

Color Renderer::RenderSubPixel(float x, float y)
{
    Ray ray = mScene.GetCamera().GetRay(x, y);
    Intersection isect;

    if (!mScene.Intersect(ray, isect))
    {
        return Color(0, 0, 0);
    }

    // 将法线向量映射到[0, 1]范围内，作为颜色输出
    return isect.normal * 0.5f + 0.5f;
}

void Renderer::RunRenderThread()
{
    const int pixelCount = mViewportWidth * mViewportHeight;

    while (mRunning.load(std::memory_order_relaxed))
    {
        int pixelIndex = mCurrentPixelIndex.fetch_add(1, std::memory_order_relaxed);
        if (pixelIndex >= pixelCount)
        {
            break;
        }

        int x = pixelIndex % mViewportWidth;
        int y = pixelIndex / mViewportWidth;

        Color color = RenderPixel(x, y);

        uint32_t r = glm::clamp((uint32_t)std::round(color.r * 255.0f), 0u, 255u);
        uint32_t g = glm::clamp((uint32_t)std::round(color.g * 255.0f), 0u, 255u);
        uint32_t b = glm::clamp((uint32_t)std::round(color.b * 255.0f), 0u, 255u);

        // 每个像素写不同槽位，多线程无需加锁
        mBuffer[static_cast<size_t>(y) * mViewportWidth + x] = (r << 16) | (g << 8) | (b);
    }
}

void Renderer::Run()
{
    mfb_window* window = mfb_open(mTitle.c_str(), static_cast<unsigned>(mViewportWidth), static_cast<unsigned>(mViewportHeight));
    if (!window)
    {
        std::cerr << "Failed to open MiniFB window\n";
        return;
    }

    mBuffer.assign(static_cast<size_t>(mViewportWidth) * mViewportHeight, 0u);
    mCurrentPixelIndex = 0;

    unsigned int workerCount = std::thread::hardware_concurrency();
    if (workerCount == 0)
    {
        workerCount = 4;
    }

    mRunning = true;
    mWorkers.reserve(workerCount);
    for (unsigned int i = 0; i < workerCount; ++i)
    {
        mWorkers.emplace_back([this]()
        {
            RunRenderThread();
        });
    }

    std::cout << "Rendering with " << workerCount << " threads\n";

    mfb_update_state state;
    do
    {
        state = mfb_update(window, mBuffer.data());
        if (state != MFB_STATE_OK)
        {
            break;
        }
    } while (mfb_wait_sync(window));

    // 关窗后通知 worker 尽快退出，再 join
    mRunning = false;
    JoinWorkers();

    mfb_close(window);
}
