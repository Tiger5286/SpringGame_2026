#pragma once
#include <memory>
#include <list>
#include "../Utility/Vector3.h"

class CollisionManager;
class CoinManager;
class EffectManager;

class Enemy;
class Player;

class EnemyManager
{
public:
	EnemyManager(CollisionManager& collisionManager,
		CoinManager& coinManager,
		EffectManager& effectManager,
		Player& player);
	virtual ~EnemyManager();

	void Init();
	void End();
	void Update();
	void Draw();

	void Spawn();

	int GetEnemyNum() { return static_cast<int>(m_pEnemies.size()); }

private:
	Vector3 GetSpawnPos();

private:
	CollisionManager& m_collisionManager;
	CoinManager& m_coinManager;
	EffectManager& m_effectManager;

	Player& m_player;

	std::list<std::shared_ptr<Enemy>> m_pEnemies;
};

