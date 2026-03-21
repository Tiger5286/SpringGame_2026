#include "GameObject.h"

GameObject::GameObject(float sphereRadius):
	m_sphere(sphereRadius)
{
}

GameObject::~GameObject()
{
	MV1DeleteModel(m_modelHandle);
}

void GameObject::SetHandle(int handle)
{
	m_modelHandle = handle;
}

Vector3 GameObject::GetPos() const
{
	return m_pos;
}
