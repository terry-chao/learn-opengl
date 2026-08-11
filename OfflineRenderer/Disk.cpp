#include "Disk.h"

#include <cmath>

Disk::Disk(SceneObject* pSceneObject, const Vector3f& normal, float radius)
    : Primitive(pSceneObject)
    , mNormal(glm::normalize(normal))
    , mRadius(radius)
{
}

bool Disk::Intersect(const Ray& ray, Intersection& isect) const
{
    const Ray localRay = TransformRayToObject(ray);

    const float denom = glm::dot(localRay.d, mNormal);
    if (std::abs(denom) < 1e-6f)
    {
        return false;
    }

    const float t = glm::dot(-localRay.o, mNormal) / denom;
    if (t < 0.0f)
    {
        return false;
    }

    const Vector3f p = localRay.o + t * localRay.d;
    if (glm::dot(p, p) > mRadius * mRadius)
    {
        return false;
    }

    isect.t = t;
    isect.position = p;
    isect.normal = mNormal;
    TransformIntersectionToWorld(ray, isect);
    return true;
}
