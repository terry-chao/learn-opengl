#pragma once

#include "Common.h"

class Primitive
{
public:
    virtual ~Primitive() = default;

    virtual bool Intersect(const Ray& ray, Intersection& isect) const = 0;
};
