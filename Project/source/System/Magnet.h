#pragma once
#include "../Utility/Vector3.h"

class Input;
class CoinManager;

class Magnet
{
public:
	Magnet(Input& input, CoinManager& coinManager);
	virtual ~Magnet();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	// 入力とコインマネージャーへの参照
	Input& m_input;
	CoinManager& m_coinManager;
};