#pragma once

#include "Common.h"

class Camera
{
public:
    void Initialize(const Vector3f& p, const Vector3f& target, const Vector3f& up,
                    float fov, float n, float f, int W, int H);

    Ray GetRay(float x, float y) const;

private:
    Vector3f mPosition{0.0f};
    Matrix4f mView{1.0f};
    Matrix4f mInvView{1.0f};
    float mFov = 0.0f;
    float mNearPlane = 0.0f;
    float mFarPlane = 0.0f;
    int mViewportWidth = 0;
    int mViewportHeight = 0;
};
