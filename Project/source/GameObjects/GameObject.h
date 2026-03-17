#pragma once
#include "../Geometry.h"

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

protected:
	Vector3 m_pos;
	int m_modelHandle = -1;
};