#include "Renderer.h"

#include "Disk.h"
#include "Sphere.h"
#include "Triangle.h"

#include <MiniFB.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <cmath>
#include <iostream>
#include <limits>

Renderer::~Renderer()
{
    mRunning = false;
    if (mWorker.joinable())
    {
        mWorker.join();
    }
}

SceneObject* Renderer::AddSceneObject(const Vector3f& position, const Vector3f& euler, float scale)
{
    mSceneObjects.push_back(std::make_unique<SceneObject>(position, euler, scale));
    return mSceneObjects.back().get();
}

Color Renderer::RenderPixel(int x, int y)
{
    const int N = SamplePerPixel;
    Color resultColor(0, 0, 0);
    for (int i = 0; i < N; ++i)
    {
        // 随机采样像素内的子像素点
        float px = x + glm::linearRand(0.0f, 1.0f);
        float py = y + glm::linearRand(0.0f, 1.0f);
        resultColor += RenderSubPixel(px, py) / static_cast<float>(N);
    }
    return resultColor;
}

Color Renderer::RenderSubPixel(float x, float y)
{
    Ray ray = mCamera.GetRay(x, y);
    Intersection closest;
    closest.t = std::numeric_limits<float>::infinity();
    bool hit = false;

    for (const auto& primitive : mPrimitives)
    {
        Intersection isect;
        if (primitive->Intersect(ray, isect) && isect.t < closest.t)
        {
            closest = isect;
            hit = true;
        }
    }

    if (!hit)
    {
        return Color(0, 0, 0);
    }

    // 将法线向量映射到[0, 1]范围内，作为颜色输出
    return closest.normal * 0.5f + 0.5f;
}

void Renderer::RunRenderThread()
{
    while (true)
    {
        int pixelIndex = mCurrentPixelIndex.fetch_add(1);
        if (pixelIndex >= mViewportWidth * mViewportHeight)
        {
            break;
        }

        int x = pixelIndex % mViewportWidth;
        int y = pixelIndex / mViewportWidth;

        Color color = RenderPixel(x, y);

        uint32_t r = glm::clamp((uint32_t)std::round(color.r * 255.0f), 0u, 255u);
        uint32_t g = glm::clamp((uint32_t)std::round(color.g * 255.0f), 0u, 255u);
        uint32_t b = glm::clamp((uint32_t)std::round(color.b * 255.0f), 0u, 255u);

        mBuffer[y * mViewportWidth + x] = (r << 16) | (g << 8) | (b);
    }
}

void Renderer::Run()
{
    mSceneObjects.clear();
    mPrimitives.clear();

    const Vector3f noRotation(0.0f);

    SceneObject* floor = AddSceneObject(Vector3f(0.0f, -1.2f, 4.0f), noRotation, 1.0f);
    mPrimitives.push_back(std::make_unique<Disk>(floor, Vector3f(0.0f, 1.0f, 0.0f), 2.5f));

    SceneObject* sphereA = AddSceneObject(Vector3f(-1.2f, 0.0f, 4.0f), noRotation, 1.0f);
    mPrimitives.push_back(std::make_unique<Sphere>(sphereA, 0.7f));

    SceneObject* sphereB = AddSceneObject(Vector3f(1.2f, 0.0f, 4.5f), noRotation, 1.0f);
    mPrimitives.push_back(std::make_unique<Sphere>(sphereB, 0.8f));

    SceneObject* sphereC = AddSceneObject(Vector3f(0.0f, 1.0f, 5.0f), noRotation, 1.0f);
    mPrimitives.push_back(std::make_unique<Sphere>(sphereC, 0.5f));

    SceneObject* triA = AddSceneObject(Vector3f(0.0f, 0.0f, 0.0f), noRotation, 1.0f);
    mPrimitives.push_back(std::make_unique<Triangle>(
        triA,
        Vector3f(-0.5f, -1.0f, 3.2f),
        Vector3f(0.5f, -1.0f, 3.2f),
        Vector3f(0.0f, 0.2f, 3.0f)));

    SceneObject* triB = AddSceneObject(Vector3f(0.0f, 0.0f, 0.0f), noRotation, 1.0f);
    mPrimitives.push_back(std::make_unique<Triangle>(
        triB,
        Vector3f(-2.0f, -1.0f, 5.0f),
        Vector3f(-1.0f, -1.0f, 5.5f),
        Vector3f(-1.5f, 0.5f, 5.0f)));

    SceneObject* triC = AddSceneObject(Vector3f(0.0f, 0.0f, 0.0f), noRotation, 1.0f);
    mPrimitives.push_back(std::make_unique<Triangle>(
        triC,
        Vector3f(1.0f, -1.0f, 3.5f),
        Vector3f(2.0f, -1.0f, 4.0f),
        Vector3f(1.5f, 0.8f, 3.8f)));

    mCamera.Initialize(
        Vector3f(0.0f, 0.0f, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        glm::radians(60.0f),
        0.1f,
        100.0f,
        mViewportWidth,
        mViewportHeight);

    mfb_window* window = mfb_open(mTitle.c_str(), static_cast<unsigned>(mViewportWidth), static_cast<unsigned>(mViewportHeight));
    if (!window)
    {
        std::cerr << "Failed to open MiniFB window\n";
        return;
    }

    mBuffer.assign(static_cast<size_t>(mViewportWidth) * mViewportHeight, 0u);
    mCurrentPixelIndex = 0;

    mRunning = true;
    mWorker = std::thread([this]()
    {
        RunRenderThread();
        mRunning = false;
    });

    mfb_update_state state;
    do
    {
        state = mfb_update(window, mBuffer.data());
        if (state != MFB_STATE_OK)
        {
            break;
        }
    } while (mfb_wait_sync(window));

    if (mWorker.joinable())
    {
        mWorker.join();
    }

    mfb_close(window);
}
