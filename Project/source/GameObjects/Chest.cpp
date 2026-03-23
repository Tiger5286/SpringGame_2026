#include "Chest.h"
#include "../Managers/CoinManager.h"

namespace
{
	constexpr float kRadius = 120.0f;

	constexpr int kAnimFrame = 18 * 2;

	constexpr int kSpawnCoinInterval = 6;
}

Chest::Chest(CoinManager& coinManager) :
	GameObject(kRadius),
	m_coinManager(coinManager)
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

	// 開くアニメーションをする
	if (m_isHitPunch)
	{
		m_hitPunchFrame++;
		if (m_hitPunchFrame < kAnimFrame)
		{
			m_anim.Update();
		}
	}
	// 開いた後コインが出てくる
	if (m_hitPunchFrame > kAnimFrame)
	{
		if (m_hitPunchFrame % kSpawnCoinInterval)
		{
			m_coinManager.Spawn(m_pos);
			m_spawnCoinCount++;
		}
	}
	// 10回出現させたら消える
	if (m_spawnCoinCount >= 10)
	{
		m_isDead = true;
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
