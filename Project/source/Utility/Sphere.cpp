#include "Sphere.h"

namespace
{
	constexpr int kDivNum = 8;
}

Sphere::Sphere(float radius):
	m_radius(radius)
{
}

Sphere::~Sphere()
{
}

void Sphere::Draw()
{
	DrawSphere3D(m_pos.ToDxLib(), m_radius, kDivNum, 0xffff00, 0xffff00, false);
}