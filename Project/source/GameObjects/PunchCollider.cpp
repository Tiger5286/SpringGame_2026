#include "PunchCollider.h"
#include "../Managers/EffectManager.h"
#include "../Managers/SoundManager.h"

namespace
{
	constexpr float kSphereRadius = 150.0f;
}

PunchCollider::PunchCollider(EffectManager& effectManager):
	GameObject(kSphereRadius),
	m_effectManager(effectManager)
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

void PunchCollider::OnCollision2(GameObject& other)
{
	if (other.GetTag() == ObjectTag::Enemy)
	{
		if (!other.IsHit())
		{
			SoundManager::GetInstance().PlaySoundGame(L"HitPunch");
			SoundManager::GetInstance().StopSound(L"Punch");
			auto effectPos = (other.GetSphere().GetPos() + m_pos) * 0.5f;
			m_effectManager.PlayEffect(L"Hit", effectPos);
			other.OnHit();
		}
	}
}
