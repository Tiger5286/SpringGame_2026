#pragma once
#include "../Utility/Vector3.h"

class ResultCoin
{
public:
	ResultCoin() = default;
	~ResultCoin() = default;

	void Init(int modelHandle);
	void End();
	void Update();
	void Draw();

	void Spawn();
private:
	int m_modelHandle = -1;

	Vector3 m_pos = { 0,0,-1000 };
	float m_angleY = 0.0f;
};

