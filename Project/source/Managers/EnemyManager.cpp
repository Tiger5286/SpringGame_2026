#include "EnemyManager.h"
#include <cassert>
#include "../GameObjects/Enemy.h"
#include "ModelManager.h"
#include "CollisionManager.h"
#include "../Game.h"

EnemyManager::EnemyManager(ModelManager& modelManager, CollisionManager& collisionManager, CoinManager& coinManager, Player& player):
	m_modelManager(modelManager),
	m_collisionManager(collisionManager),
	m_coinManager(coinManager),
	m_player(player)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
}

void EnemyManager::End()
{
	for (auto& pEnemy : m_pEnemies)
	{
		pEnemy->End();
	}
}

void EnemyManager::Update()
{
	// 死んでいる敵リスト
	std::list<std::shared_ptr<Enemy>> deadEnemies;
	// 更新と同時に死んでいる敵を探す
	for (auto& pEnemy : m_pEnemies)
	{
		pEnemy->Update();
		if (pEnemy->IsDead())
		{
			deadEnemies.push_back(pEnemy);
		}
	}
	// 死んでいる敵をリストから削除
	for (auto& pEnemy : deadEnemies)
	{
		m_collisionManager.Unregister(pEnemy);
		pEnemy->End();
		m_pEnemies.remove(pEnemy);
	}
}

void EnemyManager::Draw()
{
	for (auto& pEnemy : m_pEnemies)
	{
		pEnemy->Draw();
	}
}

void EnemyManager::SpawnEnemy()
{
	auto newEnemy = std::make_shared<Enemy>(m_player);
	newEnemy->SetHandle(m_modelManager.DuplicateModel(L"Enemy"));
	newEnemy->Init();
	newEnemy->Spawn(GetSpawnPos());
	m_collisionManager.Register(newEnemy);

	m_pEnemies.push_back(newEnemy);
}

Vector3 EnemyManager::GetSpawnPos()
{
	// 0~3のランダム(x+,x-,z+,z-)
	auto rand = GetRand(3);
	// -1000 ~ 1000のランダム
	float randPos = static_cast<float>(GetRand(Game::kFieldSize * 2) - Game::kFieldSize);

	switch (rand)
	{
	case 0:
		return Vector3(Game::kFieldSize, 0.0f, randPos);
	case 1:
		return Vector3(-Game::kFieldSize, 0.0f, randPos);
	case 2:
		return Vector3(randPos, 0.0f, Game::kFieldSize);
	case 3:
		return Vector3(randPos, 0.0f, -Game::kFieldSize);
	default :
		assert(false && "不正な値が生成されました");
		return Vector3();
	}
}
