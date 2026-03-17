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

bool Sphere::CheckCollision(const Sphere& other) const
{
	// ‹…‘Ì“¯m‚Ì‹——£‚ğZo
	float dist = (m_pos - other.m_pos).Length();
	// “–‚½‚é‹——£‚ğZo
	float hitDist = m_radius + other.m_radius;
	// ‹…‘Ì“¯m‚Ì‹——£‚ª“–‚½‚é‹——£‚ğ‰º‰ñ‚Á‚Ä‚¢‚½‚ç“–‚½‚Á‚Ä‚¢‚é
	if (dist < hitDist) return true;
	// ‚»‚¤‚Å‚È‚¢ê‡‚Í“–‚½‚Á‚Ä‚¢‚È‚¢
	return false;
}
