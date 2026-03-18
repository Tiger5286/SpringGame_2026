#pragma once
#include "../Geometry.h"
#include "../System/Animation.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() abstract;
	virtual void End() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

	// Init()より前に実行する
	void SetHandle(int handle);

	Vector3 GetPos() const;

protected:
	Vector3 m_pos;
	int m_modelHandle = -1;
};