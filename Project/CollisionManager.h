#pragma once
#include <list>

class Sphere;

// 当たり判定を管理するクラス
class CollisionManager
{
public:
	CollisionManager();
	virtual ~CollisionManager();

	void Init();
	void End();
	void Update();

	void RegisterSphere(Sphere& sphere);

private:
	std::list<Sphere&> m_spheres;
};

