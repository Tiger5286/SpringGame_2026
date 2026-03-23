#include "PunchCollider.h"

namespace
{
	constexpr float kSphereRadius = 150.0f;
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
	m_tag = ObjectTag::Punch;
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
	m_sphere.Draw(0xff0000);
#endif
}

void PunchCollider::OnCollision(const GameObject& other)
{
}
