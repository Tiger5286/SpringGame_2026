#include "CoinManager.h"
#include "../GameObjects/Coin.h"
#include "ModelManager.h"
#include "CollisionManager.h"

namespace
{
	// このフレーム数経過するまでコインには当たり判定をつけない
	constexpr int kCollidableFrame = 30;
}

CoinManager::CoinManager(ModelManager& modelManager, CollisionManager& collisionManager):
	m_modelManager(modelManager),
	m_collisionManager(collisionManager)
{
}

CoinManager::~CoinManager()
{
}

void CoinManager::Init()
{
	for (auto& coin : m_coins)
	{
		coin = std::make_shared<Coin>();
		coin->SetHandle(m_modelManager.DuplicateModel(L"Coin"));
		coin->Init();
	}
}

void CoinManager::End()
{
	for (auto& coin : m_coins)
	{
		coin->End();
	}
}

void CoinManager::Update()
{
	for (auto& coin : m_coins)
	{
		// 死んでいるコインは更新しない
		if (coin->IsDead())
		{
			continue;
		}

		coin->Update();

		// 一定時間経過したコインに当たり判定をつける
		if (coin->GetAliveFrame() == kCollidableFrame)
		{
			m_collisionManager.Register(coin);
		}

		// 当たっていたら当たり判定を登録解除
		if (coin->IsHit())
		{
			m_collisionManager.Unregister(coin);
		}

		// 取得されたコインはカウントする
		if (coin->IsGetted())
		{
			m_getCoinNum++;
		}
	}
}

void CoinManager::Draw()
{
	for (auto& coin : m_coins)
	{
		// 死んでいるコインは描画しない
		if (coin->IsDead())
		{
			continue;
		}

		// コインを描画
		coin->Draw();
	}
#ifdef _DEBUG
	DrawFormatString(100, 100, 0xffffff, L"getCoin:%d", m_getCoinNum);

	int aliveCoinNum = 0;
	for (auto& coin : m_coins)
	{
		if (!coin->IsDead())
		{
			aliveCoinNum++;
		}
	}
	DrawFormatString(100, 116, 0xffffff, L"m_coins:%d", aliveCoinNum);
#endif
}

void CoinManager::Spawn(const Vector3& pos)
{
	for (auto& coin : m_coins)
	{
		// 死んでいるコインがあればそれを再利用して召還
		if (coin->IsDead())
		{
			coin->Spawn(pos);
			return;
		}
	}
}