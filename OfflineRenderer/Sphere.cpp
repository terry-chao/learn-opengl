#include "Sphere.h"

#include <cmath>

Sphere::Sphere(SceneObject* pSceneObject, float radius)
    : Primitive(pSceneObject)
    , mRadius(radius)
{
}

bool Sphere::Intersect(const Ray& ray, Intersection& isect) const
{
    const Ray localRay = TransformRayToObject(ray);

    const Vector3f oc = localRay.o;
    const float a = glm::dot(localRay.d, localRay.d);
    const float b = 2.0f * glm::dot(oc, localRay.d);
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
    isect.position = localRay.o + t * localRay.d;
    isect.normal = glm::normalize(isect.position);
    TransformIntersectionToWorld(ray, isect);
    return true;
}
