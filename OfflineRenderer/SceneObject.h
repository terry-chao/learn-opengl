#pragma once

#include "Common.h"

Matrix4f MakeWorldTransform(const Vector3f& position, const Vector3f& euler, float scale);

class SceneObject
{
public:
    SceneObject(const Vector3f& position, const Vector3f& euler, float scale);
    virtual ~SceneObject() = default;

    const Matrix4f& GetObjectToWorld() const { return mObjectToWorld; }
    const Matrix4f& GetWorldToObject() const { return mWorldToObject; }

private:
    Matrix4f mObjectToWorld{1.0f};
    Matrix4f mWorldToObject{1.0f};
};
