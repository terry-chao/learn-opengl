#include "Triangle.h"

#include <cmath>

Triangle::Triangle(SceneObject* pSceneObject, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2)
    : Primitive(pSceneObject)
    , mV0(v0)
    , mV1(v1)
    , mV2(v2)
{
}

bool Triangle::Intersect(const Ray& ray, Intersection& isect) const
{
    const Ray localRay = TransformRayToObject(ray);

    // Moller-Trumbore
    constexpr float epsilon = 1e-6f;

    const Vector3f e1 = mV1 - mV0;
    const Vector3f e2 = mV2 - mV0;
    const Vector3f pvec = glm::cross(localRay.d, e2);
    const float det = glm::dot(e1, pvec);
    if (std::abs(det) < epsilon)
    {
        return false;
    }

    const float invDet = 1.0f / det;
    const Vector3f tvec = localRay.o - mV0;
    const float u = glm::dot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f)
    {
        return false;
    }

    const Vector3f qvec = glm::cross(tvec, e1);
    const float v = glm::dot(localRay.d, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f)
    {
        return false;
    }

    const float t = glm::dot(e2, qvec) * invDet;
    if (t < 0.0f)
    {
        return false;
    }

    isect.t = t;
    isect.position = localRay.o + t * localRay.d;
    isect.normal = glm::normalize(glm::cross(e1, e2));
    TransformIntersectionToWorld(ray, isect);
    return true;
}
