#include "GameObject.h"

namespace
{
	constexpr float kGravity = 1.0f;
	constexpr float kMaxFallSpeed = 10.0f;
}

GameObject::GameObject(float sphereRadius):
	m_sphere(sphereRadius)
{
}

GameObject::~GameObject()
{
	MV1DeleteModel(m_modelHandle);
}

void GameObject::SetHandle(int handle)
{
	m_modelHandle = handle;
}

void GameObject::Gravity()
{
	m_vel.y -= kGravity;
	if (m_vel.y < -kMaxFallSpeed)
	{
		m_vel.y = -kMaxFallSpeed;
	}

	m_pos += m_vel;
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;
		m_vel.y = 0.0f;
	}
}

Vector3 GameObject::GetPos() const
{
	return m_pos;
}
