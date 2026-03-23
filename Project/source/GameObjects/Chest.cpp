#include "Chest.h"

namespace
{
	constexpr float kRadius = 100.0f;
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
}
