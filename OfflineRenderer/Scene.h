#pragma once

#include "Camera.h"
#include "SceneObject.h"

#include <memory>
#include <string>
#include <vector>

class Scene
{
public:
    // 从 XML 加载场景；失败返回 nullptr。调用方负责 delete。
    static Scene* LoadFromXML(const std::string& filename);

    void SetCamera(const Camera& camera) { mCamera = camera; }
    const Camera& GetCamera() const { return mCamera; }

    SceneObject* CreateSceneObject(const Vector3f& position, const Vector3f& euler, float scale);

    // 返回命中的 SceneObject；未命中返回 nullptr。isect 写入最近交点信息。
    SceneObject* Intersect(Ray ray, Intersection& isect) const;

private:
    Camera mCamera;
    std::vector<std::unique_ptr<SceneObject>> mSceneObjects;
};
