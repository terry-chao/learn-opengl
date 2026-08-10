#pragma once

#include "Primitive.h"

class Triangle : public Primitive
{
public:
    Triangle(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2);

    bool Intersect(const Ray& ray, Intersection& isect) const override;

private:
    Vector3f mV0{0.0f};
    Vector3f mV1{0.0f};
    Vector3f mV2{0.0f};
};
