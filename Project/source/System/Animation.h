#pragma once
class Animation
{
public:
	Animation();
	virtual ~Animation();

	void Init(int modelHandle);
	void End();
	void Update();

	void ChangeAnim(int animHandle);

private:
	// アニメーションを行うモデルのハンドル
	int m_modelHandle = -1;

	// アニメーションのハンドル
	int m_currentAnimHandle = -1;
	int m_lastAnimHandle = -1;
	
	// アニメーション管理
	float m_currentAnimTime = 0.0f;
	float m_lastAnimTime = 0.0f;

	// アニメーションの切り替えフレーム数
	int m_animChangeFrame = 0;
};