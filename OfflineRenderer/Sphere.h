#pragma once

#include "Primitive.h"

class Sphere : public Primitive
{
public:
    Sphere(const Vector3f& center, float radius);

    bool Intersect(const Ray& ray, Intersection& isect) const override;

private:
    Vector3f mCenter{0.0f};
    float mRadius = 1.0f;
};
