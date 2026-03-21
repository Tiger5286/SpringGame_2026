#pragma once
#include "../Geometry.h"
#include "../System/Animation.h"
#include "../Utility/Sphere.h"

class GameObject
{
public:
	GameObject(float sphereRadius);
	virtual ~GameObject();

	virtual void Init() abstract;
	virtual void End() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

	// 他のオブジェクトと当たったときに呼ばれる関数
	virtual void OnCollision(const GameObject& other) abstract;

	// Init()より前に実行する
	void SetHandle(int handle);

	Vector3 GetPos() const;

	const Sphere& GetSphere() const { return m_sphere; }

protected:
	Vector3 m_pos;
	int m_modelHandle = -1;

	Sphere m_sphere;
};