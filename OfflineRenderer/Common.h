#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Vector4f = glm::vec4;

using Vector2i = glm::ivec2;
using Vector3i = glm::ivec3;
using Vector4i = glm::ivec4;

using Matrix4f = glm::mat4;

using Color = Vector3f;

struct Ray
{
    Vector3f origin{0.0f};
    Vector3f direction{0.0f, 0.0f, 1.0f};
    float maxt = 1e30f;
};

struct Intersection
{
    float t = 0.0f;
    Vector3f position{0.0f};
    Vector3f normal{0.0f, 0.0f, 1.0f};
};
