#pragma once
#include <list>
#include <memory>
#include "../Utility/Vector3.h"

class ModelManager;
class CoinManager;
class CollisionManager;

class Chest;

class ChestManager
{
public:
	ChestManager(ModelManager& modelManager,CollisionManager& collisionManager, CoinManager& coinManager);
	virtual ~ChestManager();

	void Init();
	void End();
	void Update();
	void Draw();

	void Spawn(const Vector3& playerPos);

	int GetChestNum() { return static_cast<int>(m_chests.size()); }

private:
	ModelManager& m_modelManager;
	CollisionManager& m_collisionManager;
	CoinManager& m_coinManager;

	std::list<std::shared_ptr<Chest>> m_chests;
};

