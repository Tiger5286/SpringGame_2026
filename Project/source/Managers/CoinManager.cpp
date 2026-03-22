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
	for (auto& coin : m_coins)
	{
		coin->Update();
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