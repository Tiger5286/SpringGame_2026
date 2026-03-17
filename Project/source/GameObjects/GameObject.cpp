#include "GameObject.h"

GameObject::GameObject()
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