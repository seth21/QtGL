#include "pointlight.h"

PointLight::PointLight()
{
}

float PointLight::getRadius(float exposure)
{
	float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
	float radius =
		(-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (exposure * 256.0 / 3.0) * lightMax)))
		/ (2 * quadratic);
	return radius;
}

void PointLight::createShadowMap()
{
}
