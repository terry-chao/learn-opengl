#pragma once

#include <cfloat>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Vector4f = glm::vec4;

using Vector2i = glm::ivec2;
using Vector3i = glm::ivec3;
using Vector4i = glm::ivec4;

using Matrix3f = glm::mat3;
using Matrix4f = glm::mat4;

using Color = Vector3f;

constexpr float PI = 3.14159265358979323846f;
constexpr float INV_PI = 1.0f / PI;

class Material;

struct Ray
{
    Vector3f o{0.0f};
    Vector3f d{0.0f, 0.0f, 1.0f};

    float mint = 0.0f;
    float maxt = FLT_MAX;
};

struct Intersection
{
    float t = 0.0f;
    Vector3f position{0.0f};
    Vector3f normal{0.0f, 0.0f, 1.0f};
    const Material* material = nullptr;
};
