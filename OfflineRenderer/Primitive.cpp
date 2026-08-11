#include "Primitive.h"

#include "SceneObject.h"

Primitive::Primitive(SceneObject* pSceneObject)
    : m_pSceneObject(pSceneObject)
{
}

Ray Primitive::TransformRayToObject(const Ray& worldRay) const
{
    if (!m_pSceneObject)
    {
        return worldRay;
    }

    const Matrix4f& worldToObject = m_pSceneObject->GetWorldToObject();
    Ray objectRay;
    objectRay.o = Vector3f(worldToObject * Vector4f(worldRay.o, 1.0f));
    objectRay.d = Vector3f(worldToObject * Vector4f(worldRay.d, 0.0f));
    return objectRay;
}

void Primitive::TransformIntersectionToWorld(const Ray& worldRay, Intersection& isect) const
{
    if (!m_pSceneObject)
    {
        return;
    }

    const Matrix4f& objectToWorld = m_pSceneObject->GetObjectToWorld();
    const Matrix4f& worldToObject = m_pSceneObject->GetWorldToObject();

    isect.position = Vector3f(objectToWorld * Vector4f(isect.position, 1.0f));
    isect.normal = glm::normalize(Vector3f(glm::transpose(worldToObject) * Vector4f(isect.normal, 0.0f)));
    isect.t = glm::length(isect.position - worldRay.o);
}
