#pragma once
#include <list>
#include <memory>
#include "../Utility/Vector3.h"

class ModelManager;
class CollisionManager;

class Coin;

class CoinManager
{
public:
	CoinManager(ModelManager& modelManager,CollisionManager& collisionManager);
	~CoinManager();

	void Init();
	void End();
	void Update();
	void Draw();

	void Spawn(const Vector3& pos);

private:
	ModelManager& m_modelManager;
	CollisionManager& m_collisionManager;

	std::list<std::shared_ptr<Coin>> m_coins;
};