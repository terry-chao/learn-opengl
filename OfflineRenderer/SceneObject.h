#pragma once

#include "Common.h"
#include "Primitive.h"

#include <memory>
#include <vector>

Matrix4f MakeWorldTransform(const Vector3f& position, const Vector3f& euler, float scale);

class SceneObject
{
public:
    SceneObject(const Vector3f& position, const Vector3f& euler, float scale);
    virtual ~SceneObject() = default;

    virtual bool Intersect(const Ray& ray, Intersection& isect) const;

    const Matrix4f& GetObjectToWorld() const { return mObjectToWorld; }
    const Matrix4f& GetWorldToObject() const { return mWorldToObject; }

    template<typename T, typename... Args>
    T* CreatePrimitive(Args&&... args);

    void AddPrimitive(std::unique_ptr<Primitive> primitive);
    const std::vector<std::unique_ptr<Primitive>>& GetPrimitives() const { return mPrimitives; }

private:
    Matrix4f mObjectToWorld{1.0f};
    Matrix4f mWorldToObject{1.0f};
    std::vector<std::unique_ptr<Primitive>> mPrimitives;
};

template<typename T, typename... Args>
inline T* SceneObject::CreatePrimitive(Args&&... args)
{
    // 所有权交给 mPrimitives；返回的裸指针只是非拥有的观察指针，方便调用方继续配置
    auto primitive = std::make_unique<T>(this, std::forward<Args>(args)...);
    T* raw = primitive.get();
    mPrimitives.push_back(std::move(primitive));
    return raw;
}
