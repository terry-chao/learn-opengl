#pragma once

#include "Common.h"

class SceneObject;

class Primitive
{
public:
    explicit Primitive(SceneObject* pSceneObject);
    virtual ~Primitive() = default;

    virtual bool Intersect(const Ray& ray, Intersection& isect) const = 0;

protected:
    Ray TransformRayToObject(const Ray& worldRay) const;
    void TransformIntersectionToWorld(const Ray& worldRay, Intersection& isect) const;

    SceneObject* GetSceneObject() const { return m_pSceneObject; }

private:
    SceneObject* m_pSceneObject = nullptr;
};
