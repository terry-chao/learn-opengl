#pragma once

#include "Primitive.h"

class Disk : public Primitive
{
public:
    Disk(SceneObject* pSceneObject, const Vector3f& normal, float radius);

    bool Intersect(const Ray& ray, Intersection& isect) const override;

private:
    Vector3f mNormal{0.0f, 0.0f, 1.0f};
    float mRadius = 1.0f;
};
