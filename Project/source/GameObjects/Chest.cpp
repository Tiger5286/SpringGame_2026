#include "Chest.h"

namespace
{
	constexpr float kRadius = 120.0f;

	constexpr int kAnimFrame = 18 * 2;
}

Chest::Chest() :
	GameObject(kRadius)
{
}

Chest::~Chest()
{
}

void Chest::Init()
{
	m_anim.Init(m_modelHandle, L"Chest_Open");
}

void Chest::End()
{
}

void Chest::Update()
{
	m_sphere.SetPos(Vector3(m_pos.x, m_pos.y + kRadius, m_pos.z));

	if (m_isHitPunch)
	{
		m_anim.Update();
	}
}

void Chest::Draw()
{
	MV1DrawModel(m_modelHandle);
#ifdef _DEBUG
	m_sphere.Draw();
#endif
}

void Chest::OnCollision(const GameObject& other)
{
	if (other.GetTag() == ObjectTag::Punch)
	{
		m_isHitPunch = true;
	}
}
