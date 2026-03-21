#pragma once
#include <list>
#include <memory>

class GameObject;

// 当たり判定を管理するクラス
class CollisionManager
{
public:
	CollisionManager();
	virtual ~CollisionManager();

	void Init();
	void End();
	void Update();

	void Register(std::shared_ptr<GameObject> pGameObject);
	void Unregister(std::shared_ptr<GameObject> pGameObject);

private:
	bool CheckCollision(const GameObject& a, const GameObject& b) const;

private:
	std::list<std::shared_ptr<GameObject>> m_pGameObjects;
};

