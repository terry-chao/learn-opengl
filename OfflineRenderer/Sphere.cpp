#include "Sphere.h"

#include <cmath>

Sphere::Sphere(const Vector3f& center, float radius)
    : mCenter(center)
    , mRadius(radius)
{
}

bool Sphere::Intersect(const Ray& ray, Intersection& isect) const
{
    const Vector3f oc = ray.origin - mCenter;
    const float a = glm::dot(ray.direction, ray.direction);
    const float b = 2.0f * glm::dot(oc, ray.direction);
    const float c = glm::dot(oc, oc) - mRadius * mRadius;
    const float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
    {
        return false;
    }

    const float sqrtD = std::sqrt(discriminant);
    float t = (-b - sqrtD) / (2.0f * a);
    if (t < 0.0f)
    {
        t = (-b + sqrtD) / (2.0f * a);
        if (t < 0.0f)
        {
            return false;
        }
    }

    isect.t = t;
    isect.position = ray.origin + t * ray.direction;
    isect.normal = glm::normalize(isect.position - mCenter);
    return true;
}
