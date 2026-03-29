#pragma once
#include <array>
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

	int GetCoinNum() const { return m_getCoinNum; }

private:
	// コインの存在上限数
	static constexpr int kCoinMaxNum = 150;

private:
	ModelManager& m_modelManager;
	CollisionManager& m_collisionManager;

	std::array<std::shared_ptr<Coin>, kCoinMaxNum> m_coins;
	int m_getCoinNum = 0;
};