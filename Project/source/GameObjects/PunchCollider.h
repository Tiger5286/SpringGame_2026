#pragma once
#include "GameObject.h"

class EffectManager;

class PunchCollider : public GameObject
{
public:
	PunchCollider(EffectManager& effectManager);
	virtual ~PunchCollider() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;
	
	void OnCollision(const GameObject& other) override;
	void OnCollision2(GameObject& other) override;

	void SetPos(const Vector3& pos)
	{
		m_pos = pos;
		m_sphere.SetPos(pos);
	}

private:
	EffectManager& m_effectManager;
};

