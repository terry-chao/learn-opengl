#include "Renderer.h"

#include <MiniFB.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <random>

#include "Common.h"
#include "Material.h"

namespace
{
// 以法线为局部 +Z 轴，构造正交坐标系（列向量为局部 x/y/z 在世界中的方向）
Matrix3f MakeCoordinateSystem(const Vector3f& normal)
{
    const Vector3f n = glm::normalize(normal);
    const Vector3f up = (std::abs(n.y) < 0.999f) ? Vector3f(0.0f, 1.0f, 0.0f) : Vector3f(1.0f, 0.0f, 0.0f);
    const Vector3f t = glm::normalize(glm::cross(up, n));
    const Vector3f b = glm::cross(n, t);
    return Matrix3f(t, b, n);
}
} // namespace

Renderer::Renderer()
{
    Scene* scene = Scene::LoadFromXML("assets/scene07.xml");
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

Color Renderer::GetIrradiance(const Ray& ray)
{
    Intersection isect;
    if (!mScene.Intersect(ray, isect))
        return Color(0, 0, 0);

    Color E(0, 0, 0);

    // E(p) = Σ Li * cosθ * V
    for (auto& pLight : mScene.GetLights())
    {
        Vector3f sourcePos;
        Color L = pLight->GetRadiance(isect.position, sourcePos);

        // 求shadowRay
        Ray shadowRay;
        shadowRay.o = isect.position;
        shadowRay.d = glm::normalize(sourcePos - isect.position);
        shadowRay.mint = 1e-3f;
        shadowRay.maxt = glm::length(sourcePos - isect.position);

        Intersection shadow_isect;
        if (mScene.Intersect(shadowRay, shadow_isect)) // 如果shadowRay与场景中的物体相交，说明该点被遮挡了
            continue;

        float cosTheta = glm::dot(isect.normal, shadowRay.d);
        E += L * glm::max(cosTheta, 0.0f);
    }

    return E;
}

Color Renderer::GetRadiance(const Ray& ray)
{
    Intersection isect;
    SceneObject* pSceneObject = mScene.Intersect(ray, isect);
    if (pSceneObject == nullptr)
        return Color(0, 0, 0);

    const Material* pMaterial = pSceneObject->GetMaterial();
    Color Lo(0, 0, 0);

    Matrix3f localToWorld = MakeCoordinateSystem(isect.normal);
    Matrix3f worldToLocal = glm::transpose(localToWorld);

    Vector3f wo = worldToLocal * (-ray.d); // 出射方向，转换到局部坐标系

    for (const auto& pLight : mScene.GetLights())
    {
        Vector3f sourcePos;
        Color L = pLight->GetRadiance(isect.position, sourcePos);

        // 求shadowRay
        Ray shadowRay;
        shadowRay.o = isect.position;
        shadowRay.d = glm::normalize(sourcePos - isect.position);
        shadowRay.mint = 1e-3f;
        shadowRay.maxt = glm::length(sourcePos - isect.position);

        Intersection shadow_isect;
        if (mScene.Intersect(shadowRay, shadow_isect)) // 如果shadowRay与场景中的物体相交，说明该点被遮挡了
            continue;

        Vector3f wi = worldToLocal * shadowRay.d; // 入射方向，转换到局部坐标系
        float cosTheta = glm::dot(isect.normal, shadowRay.d);
        Color brdf = pMaterial->BRDF(wo, wi);
        Lo += brdf * L * glm::max(cosTheta, 0.0f);
    }

    return Lo;
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
    // 每线程独立 RNG，避免依赖 glm::linearRand
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // SSAA
    const int N = SamplePerPixel; // 每个像素采样的次数
    Color resultColor(0, 0, 0);

    for (int i = 0; i < N; i++)
    {
        // (x, y) - (x+1, y+1)范围内随机采样一个点：
        float px = x + dist(rng);
        float py = y + dist(rng);

        Color color = RenderSubPixel(px, py);
        resultColor += (color / (float)N);
    }

    return resultColor; // 取平均值，得到最终颜色	
}

Color Renderer::RenderSubPixel(float x, float y)
{
    Ray ray = mScene.GetCamera().GetRay(x, y);
    //Intersection isect;

    //if (!mScene.Intersect(ray, isect))
    //{
    //    return Color(0, 0, 0);
    //}
    // 将法线向量映射到[0, 1]范围内，作为颜色输出
    //return isect.normal * 0.5f + 0.5f;

    return GetIrradiance(ray);
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
