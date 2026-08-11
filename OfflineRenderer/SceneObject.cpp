#include "SceneObject.h"

#include <glm/gtc/matrix_transform.hpp>

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
}
