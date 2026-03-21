#include "CollisionManager.h"
#include "../Utility/Sphere.h"
#include "../GameObjects/GameObject.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Init()
{
}

void CollisionManager::End()
{
}

void CollisionManager::Update()
{
	for (auto& objA : m_pGameObjects)
	{
		for (auto& objB : m_pGameObjects)
		{
			// 自分自身との当たり判定は行わない
			if (objA == objB) continue;
			// 当たっているかチェック
			if (CheckCollision(*objA, *objB))
			{
				objA->OnCollision(*objB);
				objB->OnCollision(*objA);
			}
		}
	}
}

void CollisionManager::Register(std::shared_ptr<GameObject> pGameObject)
{
	m_pGameObjects.push_back(pGameObject);
}

void CollisionManager::Unregister(std::shared_ptr<GameObject> pGameObject)
{
	m_pGameObjects.remove(pGameObject);
}

bool CollisionManager::CheckCollision(const GameObject& a, const GameObject& b) const
{
	// 球同士の距離を算出
	float dist = (a.GetSphere().GetPos() - b.GetSphere().GetPos()).SquaredLength();
	// 球同士が当たる距離を算出
	float hitDist = a.GetSphere().GetRadius() + b.GetSphere().GetRadius();
	// 距離が当たる距離以下なら当たっている
	if (dist <= hitDist * hitDist) return true;
	return false;
}
