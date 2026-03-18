#pragma once
#include "../Utility/Vector3.h"

class Player;

class Camera
{
public:
	Camera(Player& player);
	virtual ~Camera();

	void Init();
	void End();
	void Update();

private:

	Player& m_player;

	Vector3 m_pos;
	Vector3 m_target;
	float angle = 0.0f;
};

