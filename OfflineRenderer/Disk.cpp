#include "Disk.h"

#include <cmath>

Disk::Disk(const Vector3f& center, const Vector3f& normal, float radius)
    : mCenter(center)
    , mNormal(glm::normalize(normal))
    , mRadius(radius)
{
}

bool Disk::Intersect(const Ray& ray, Intersection& isect) const
{
    const float denom = glm::dot(ray.direction, mNormal);
    if (std::abs(denom) < 1e-6f)
    {
        return false;
    }

    const float t = glm::dot(mCenter - ray.origin, mNormal) / denom;
    if (t < 0.0f)
    {
        return false;
    }

    const Vector3f p = ray.origin + t * ray.direction;
    const Vector3f d = p - mCenter;
    if (glm::dot(d, d) > mRadius * mRadius)
    {
        return false;
    }

    isect.t = t;
    isect.position = p;
    isect.normal = mNormal;
    return true;
}
