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

private:
	Vector3 m_pos;
};