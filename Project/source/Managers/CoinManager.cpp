#include "CoinManager.h"
#include "../GameObjects/Coin.h"
#include "ModelManager.h"
#include "CollisionManager.h"

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
	// 死んでいるコインリスト
	std::list<std::shared_ptr<Coin>> deadCoins;
	for (auto& coin : m_coins)
	{
		coin->Update();
		// 当たっていたら当たり判定を登録解除
		if (coin->IsHit())
		{
			m_collisionManager.Unregister(coin);
		}
		// 死んでいたら死亡リストに追加
		if (coin->IsDead())
		{
			deadCoins.push_back(coin);
		}
	}
	// 死んでいるコインをリストから削除
	for (auto& coin : deadCoins)
	{
		m_collisionManager.Unregister(coin);
		coin->End();
		m_coins.remove(coin);
	}
}

void CoinManager::Draw()
{
	for (auto& coin : m_coins)
	{
		coin->Draw();
	}
}

void CoinManager::Spawn(const Vector3& pos)
{
	auto newCoin = std::make_shared<Coin>();
	newCoin->SetHandle(m_modelManager.DuplicateModel(L"Coin"));
	newCoin->Init();
	m_collisionManager.Register(newCoin);
	newCoin->Spawn(pos);
	m_coins.push_back(newCoin);
}