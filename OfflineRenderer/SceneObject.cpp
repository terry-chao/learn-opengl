#include "SceneObject.h"

#include <glm/gtc/matrix_transform.hpp>

#include <limits>

Matrix4f MakeWorldTransform(const Vector3f& position, const Vector3f& euler, float scale)
{
    const Matrix4f T = glm::translate(Matrix4f(1.0f), position);
    const Matrix4f Rx = glm::rotate(Matrix4f(1.0f), euler.x, Vector3f(1.0f, 0.0f, 0.0f));
    const Matrix4f Ry = glm::rotate(Matrix4f(1.0f), euler.y, Vector3f(0.0f, 1.0f, 0.0f));
    const Matrix4f Rz = glm::rotate(Matrix4f(1.0f), euler.z, Vector3f(0.0f, 0.0f, 1.0f));
    const Matrix4f S = glm::scale(Matrix4f(1.0f), Vector3f(scale));
    return T * Rz * Ry * Rx * S;
}

SceneObject::SceneObject(const Vector3f& position, const Vector3f& euler, float scale)
{
    mObjectToWorld = MakeWorldTransform(position, euler, scale);
    mWorldToObject = glm::inverse(mObjectToWorld);
    // 默认白色兰伯特材质
    CreateMaterial<LambertMaterial>(Color(1.0f, 1.0f, 1.0f));
}

void SceneObject::AddPrimitive(std::unique_ptr<Primitive> primitive)
{
    mPrimitives.push_back(std::move(primitive));
}

void SceneObject::SetMaterial(std::unique_ptr<Material> material)
{
    mMaterial = std::move(material);
}

bool SceneObject::Intersect(const Ray& ray, Intersection& isect) const
{
    Intersection closest;
    closest.t = std::numeric_limits<float>::infinity();
    bool hit = false;

    for (const auto& primitive : mPrimitives)
    {
        Intersection candidate;
        if (primitive->Intersect(ray, candidate)
            && candidate.t >= ray.mint
            && candidate.t < closest.t
            && candidate.t < ray.maxt)
        {
            closest = candidate;
            hit = true;
        }
    }

    if (hit)
    {
        isect = closest;
    }
    return hit;
}
