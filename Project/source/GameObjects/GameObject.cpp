#include "GameObject.h"
#include "../Game.h"

namespace
{
	constexpr float kGravity = 1.0f;
	constexpr float kMaxFallSpeed = 10.0f;

	constexpr float kResistance = 0.5f;
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

void GameObject::Resistance()
{
	// x方向の抵抗
	if (m_vel.x > kResistance)
	{
		m_vel.x -= kResistance;
	}
	else if (m_vel.x < -kResistance)
	{
		m_vel.x += kResistance;
	}
	else
	{
		m_vel.x = 0.0f;
	}
	// z方向の抵抗
	if (m_vel.z > kResistance)
	{
		m_vel.z -= kResistance;
	}
	else if (m_vel.z < -kResistance)
	{
		m_vel.z += kResistance;
	}
	else
	{
		m_vel.z = 0.0f;
	}
}

void GameObject::LimitPos()
{
	// 移動範囲の制限
	if (m_pos.x > Game::kFieldSize) m_pos.x = Game::kFieldSize;
	if (m_pos.x < -Game::kFieldSize) m_pos.x = -Game::kFieldSize;
	if (m_pos.z > Game::kFieldSize) m_pos.z = Game::kFieldSize;
	if (m_pos.z < -Game::kFieldSize) m_pos.z = -Game::kFieldSize;
}

Vector3 GameObject::GetPos() const
{
	return m_pos;
}
