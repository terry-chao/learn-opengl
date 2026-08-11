#pragma once

#include "Primitive.h"

class Sphere : public Primitive
{
public:
    Sphere(SceneObject* pSceneObject, float radius);

    bool Intersect(const Ray& ray, Intersection& isect) const override;

private:
    float mRadius = 1.0f;
};
