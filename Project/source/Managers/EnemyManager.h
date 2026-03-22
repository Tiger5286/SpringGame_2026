#pragma once
#include <memory>
#include <list>
#include "../Utility/Vector3.h"

class ModelManager;
class CollisionManager;
class CoinManager;

class Enemy;
class Player;

class EnemyManager
{
public:
	EnemyManager(ModelManager& modelManager,
		CollisionManager& collisionManager,
		CoinManager& coinManager,
		Player& player);
	virtual ~EnemyManager();

	void Init();
	void End();
	void Update();
	void Draw();

	void SpawnEnemy();

private:
	Vector3 GetSpawnPos();

private:
	ModelManager& m_modelManager;
	CollisionManager& m_collisionManager;
	CoinManager& m_coinManager;
	Player& m_player;

	std::list<std::shared_ptr<Enemy>> m_pEnemies;
};

