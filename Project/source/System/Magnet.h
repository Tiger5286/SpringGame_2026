#pragma once

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

	int m_graphHandle = -1;	// 引き寄せのアイコンのグラフィックハンドル

	int m_cooldown = 0; // 引き寄せのクールダウンタイム
};