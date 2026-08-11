#pragma once

#include "Common.h"

class Material
{
public:
    virtual ~Material() = default;

    virtual Color BRDF(const Vector3f& wo, const Vector3f& wi) const = 0;
};

class LambertMaterial : public Material
{
public:
    explicit LambertMaterial(const Color& albedo);

    Color BRDF(const Vector3f& wo, const Vector3f& wi) const override;

private:
    Color mAlbedo;
};
