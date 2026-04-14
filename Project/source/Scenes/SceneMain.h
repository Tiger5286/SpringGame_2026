#pragma once
#include "SceneBase.h"
#include <memory>

class Camera;

class ModelManager;
class CollisionManager;
class EnemyManager;
class CoinManager;
class ChestManager;
class EffectManager;

class Player;

class SkyBox;

class SceneMain : public SceneBase
{
public:
	SceneMain(Input& input);
	~SceneMain() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	int GetScore() const { return m_score; }
private:
	void UpdateStart();
	void DrawStart();
	void DrawFinish();

	void DrawGrid();

private:
	int m_frameCount = 0;

	// 操作説明の画像のハンドル
	int m_howToPlayGraphHandle = -1;

	std::shared_ptr<Camera> m_pCamera = nullptr;

	std::shared_ptr<CollisionManager> m_pCollisionManager = nullptr;
	std::shared_ptr<EnemyManager> m_pEnemyManager = nullptr;
	std::shared_ptr<CoinManager> m_pCoinManager = nullptr;
	std::shared_ptr<ChestManager> m_pChestManager = nullptr;
	std::shared_ptr<EffectManager> m_pEffectManager = nullptr;

	std::shared_ptr<Player> m_pPlayer = nullptr;

	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;

	// シャドウマップのハンドル
	int m_shadowMapHandle = -1;

	// フォントのハンドル
	int m_titleFontHandle = -1;
	int m_uiFontHandle = -1;

	// ゲームのスコア
	int m_score = 0;
	int m_dispScore = 0;	// 表示用スコア

	// ゲーム開始のフラグとカウント
	bool m_isStarted = false;
	int m_startCount = 0;

	// ゲーム中のカウント
	int m_gameCount = 0;

	// ゲーム終了のカウント
	int m_finishCount = 0;
};

