#pragma once
#include "SceneBase.h"
#include "../Utility/Vector3.h"
#include <memory>

class Camera;
class SkyBox;
class Player;

class SceneTitle : public SceneBase
{
public:
	SceneTitle(Input& input,SceneManager& sceneManager);
	virtual ~SceneTitle();

	virtual void Init() override;
	virtual void End() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	void DrawStartText();

	void ControlCamera();

private:
	int m_frameCount = 0;

	// Aボタンが押されたあとのフレーム数
	int m_pressStartFrameCount = 0;

	// フォントのハンドル
	int m_fontHandle = -1;

	// タイトルの画像のハンドル
	int m_titleGraphHandle = -1;
	int m_titleBackGraphHandle = -1;
	// タイトルの画像の回転角度
	float m_titleBackGraphAngle = 0.0f;

	// シャドウマップのハンドル
	int m_shadowMapHandle = -1;

	// スカイボックスとプレイヤー
	std::shared_ptr<SkyBox> m_pSkyBox;
	std::shared_ptr<Player> m_pPlayer;

	// カメラの注視点
	Vector3 m_cameraTarget;
};

