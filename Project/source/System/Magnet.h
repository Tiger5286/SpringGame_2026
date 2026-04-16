#pragma once
#include <vector>

class Input;
class Player;
class CoinManager;

class Magnet
{
public:
	Magnet(Input& input,Player& player, CoinManager& coinManager);
	virtual ~Magnet();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	// 参照たち
	Input& m_input;
	Player& m_player;
	CoinManager& m_coinManager;

	int m_frameCount = 0;

	std::vector<int> m_graphHandles;	// グラフィックハンドル

	float m_uiBackAngle = 0.0f;	// アイコンの背景の角度

	int m_cooldown = 0; // 引き寄せのクールダウンタイム
};