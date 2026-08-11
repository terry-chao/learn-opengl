#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

void Camera::Initialize(const Vector3f& p, const Vector3f& target, const Vector3f& up,
                        float fov, float n, float f, int W, int H)
{
    mPosition = p;
    mView = glm::lookAtLH(p, target, up);
    mInvView = glm::inverse(mView);
    mFov = fov;
    mNearPlane = n;
    mFarPlane = f;
    mViewportWidth = W;
    mViewportHeight = H;
}

Ray Camera::GetRay(float x, float y) const
{
    const float aspect = static_cast<float>(mViewportWidth) / static_cast<float>(mViewportHeight);
    const float tanHalfFov = std::tan(mFov * 0.5f);

    const float px = (2.0f * x / static_cast<float>(mViewportWidth) - 1.0f) * aspect * tanHalfFov;
    const float py = (1.0f - 2.0f * y / static_cast<float>(mViewportHeight)) * tanHalfFov;

    // 左手坐标系：相机朝向 +Z
    const Vector3f dirView = glm::normalize(Vector3f(px, py, 1.0f));
    const Vector3f dirWorld = glm::normalize(Vector3f(mInvView * Vector4f(dirView, 0.0f)));

    Ray ray;
    ray.o = mPosition;
    ray.d = dirWorld;
    return ray;
}
