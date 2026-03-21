#include "PunchCollider.h"

namespace
{
	constexpr float kSphereRadius = 100.0f;
}

PunchCollider::PunchCollider():
	GameObject(kSphereRadius)
{
}

PunchCollider::~PunchCollider()
{
}

void PunchCollider::Init()
{
}

void PunchCollider::End()
{
}

void PunchCollider::Update()
{
}

void PunchCollider::Draw()
{
#ifdef _DEBUG
	m_sphere.Draw();
#endif
}

void PunchCollider::OnCollision(const GameObject& other)
{
}
