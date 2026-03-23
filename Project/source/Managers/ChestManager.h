#pragma once
#include <list>
#include <memory>
#include "../Utility/Vector3.h"

class ModelManager;
class CoinManager;
class Chest;

class ChestManager
{
public:
	ChestManager(ModelManager& modelManager, CoinManager& coinManager);
	virtual ~ChestManager();

	void Init();
	void End();
	void Update();
	void Draw();

	void Spawn(const Vector3& playerPos);

private:
	ModelManager& m_modelManager;
	CoinManager& m_coinManager;

	std::list<std::shared_ptr<Chest>> m_chests;
};

