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

private:

};

