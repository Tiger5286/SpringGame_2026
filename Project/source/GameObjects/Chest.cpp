#include "Chest.h"

namespace
{
	constexpr float kRadius = 120.0f;
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
}

void Chest::End()
{
}

void Chest::Update()
{
	m_sphere.SetPos(Vector3(m_pos.x, m_pos.y + kRadius, m_pos.z));
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

	}
}
