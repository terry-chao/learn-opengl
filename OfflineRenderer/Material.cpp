#include "Material.h"

LambertMaterial::LambertMaterial(const Color& albedo)
    : mAlbedo(albedo)
{
}

Color LambertMaterial::BRDF(const Vector3f& wo, const Vector3f& wi) const
{
    (void)wo;
    (void)wi;
    return mAlbedo * INV_PI;
}
