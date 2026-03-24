#pragma once
#include "GameObject.h"

class PunchCollider : public GameObject
{
public:
	PunchCollider();
	virtual ~PunchCollider() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;
	
	void OnCollision(const GameObject& other) override;

	void SetPos(const Vector3& pos)
	{
		m_pos = pos;
		m_sphere.SetPos(pos);
	}

private:

};

