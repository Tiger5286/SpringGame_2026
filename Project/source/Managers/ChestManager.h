#pragma once
#include <list>
#include <memory>
#include "../Utility/Vector3.h"

class CoinManager;
class CollisionManager;
class EffectManager;

class Chest;

class ChestManager
{
public:
	ChestManager(CollisionManager& collisionManager,
		CoinManager& coinManager,
		EffectManager& effectManager);
	virtual ~ChestManager();

	void Init();
	void End();
	void Update();
	void Draw();

	void Spawn(const Vector3& playerPos);

	int GetChestNum() { return static_cast<int>(m_chests.size()); }

private:
	CollisionManager& m_collisionManager;
	CoinManager& m_coinManager;
	EffectManager& m_effectManager;

	std::list<std::shared_ptr<Chest>> m_chests;
};

