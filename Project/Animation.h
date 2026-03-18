#pragma once
class Animation
{
public:
	Animation();
	virtual ~Animation();

	void Init();
	void End();
	void Update();

private:
	// アニメーションを行うモデルのハンドル
	int m_modelHandle = -1;

	// アニメーションのハンドル
	int m_currentAnimHandle = -1;
	int m_lastAnimHandle = -1;
};