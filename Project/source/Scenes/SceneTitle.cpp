#include "SceneTitle.h"
#include "Dxlib.h"
#include "../System/Input.h"
#include <string>
#include "../Game.h"
#include <cassert>
#include "../System/SkyBox.h"
#include "../Managers/ModelManager.h"
#include "../GameObjects/Player.h"
#include "../Managers/SoundManager.h"
#include "SceneManager.h"
#include "SceneMain.h"

namespace
{
	const Vector3 kCameraPos = Vector3(0.0f, 300.0f, -1300.0f);
	const Vector3 kCameraTarget = Vector3(0.0f, 500.0f, 0.0f);
	const Vector3 kLightDirection = Vector3(0.0f, -1.5f, 1.0f);

	constexpr int kTitleY = 200;
	constexpr float kTitleScale = 0.5f;
	constexpr float kTitleRotateSpeed = 0.03f;
}

SceneTitle::SceneTitle(Input& input, SceneManager& sceneManager):
	SceneBase(input,sceneManager)
{
	m_sceneType = SceneType::Title;
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	// フォントの生成
	m_fontHandle = CreateFontToHandle(Game::kFontName, 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_fontHandle != -1 && "フォントが正しく生成されませんでした");
	// タイトルの画像の読み込み
	m_titleGraphHandle = LoadGraph(L"data/Graphs/title.png");
	m_titleBackGraphHandle = LoadGraph(L"data/Graphs/titleBack.png");

	// シャドウマップの生成
	m_shadowMapHandle = MakeShadowMap(8192, 8192);

	// カメラの設定
	SetCameraPositionAndTarget_UpVecY(kCameraPos.ToDxLib(), kCameraTarget.ToDxLib());

	// プレイヤーの生成と初期化
	m_pPlayer = std::make_shared<Player>(m_input,nullptr,nullptr);
	m_pPlayer->SetHandle(ModelManager::GetInstance().DuplicateModel(L"Player"));
	m_pPlayer->Init();

	// スカイボックスの生成と初期化
	m_pSkyBox = std::make_shared<SkyBox>();
	m_pSkyBox->Init();
	m_pSkyBox->SetCameraPos(kCameraPos);

	// ライトの設定
	auto cameraToPlayer = m_pPlayer->GetPos() - kCameraPos;
	SetLightDirection(cameraToPlayer.ToDxLib());

	// BGMを再生
	SoundManager::GetInstance().PlaySoundGame(L"TitleBGM", true, true);
}

void SceneTitle::End()
{
	// フォントの削除
	DeleteFontToHandle(m_fontHandle);
	// 画像の削除
	DeleteGraph(m_titleGraphHandle);
	DeleteGraph(m_titleBackGraphHandle);

	m_pPlayer->End();

	m_pSkyBox->End();
}

void SceneTitle::Update()
{
	m_frameCount++;
	// Aボタンが押されたあとのフレーム数をカウント
	if (m_pressStartFrameCount > 0)
	{
		m_pressStartFrameCount++;
	}
	// Aボタンが押されたあと、一定フレーム数経過したらシーンを終了
	if (m_pressStartFrameCount > 40)
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_input,m_sceneManager));
		return;
	}

	m_pPlayer->Update();

	if (m_input.IsTriggerd(XINPUT_BUTTON_A))
	{
		// Aボタンが押された瞬間からフレームをカウント
		if (m_pressStartFrameCount == 0)
		{
			m_pressStartFrameCount = 1;
			SoundManager::GetInstance().PlaySoundGame(L"Decision");
			SoundManager::GetInstance().StopSound(L"TitleBGM", true);
		}
	}

	m_pSkyBox->Update();

	// タイトルの画像の回転
	m_titleBackGraphAngle += kTitleRotateSpeed;

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_1))
	{
		SoundManager::GetInstance().StopSound(L"TitleBGM", true);
		m_sceneManager.ChangeScene(std::make_shared<SceneMain>(m_input, m_sceneManager));
		return;
	}
#endif
}

void SceneTitle::Draw()
{
	m_pSkyBox->Draw();
	// シャドウマップに描画
	ShadowMap_DrawSetup(m_shadowMapHandle);

	m_pPlayer->Draw();

	ShadowMap_DrawEnd();

	// 通常描画
	m_pPlayer->Draw();

	// 床だけ特定のライトの方向で描画する
	SetLightDirection(kLightDirection.ToDxLib());
	// シャドウマップを使用する
	SetUseShadowMap(0, m_shadowMapHandle);
	// 床の描画
	MV1DrawModel(ModelManager::GetInstance().GetModelHandle(L"Floor"));
	// シャドウマップの設定を解除
	SetUseShadowMap(0, -1);
	// ライトを元に戻す
	auto cameraToPlayer = m_pPlayer->GetPos() - kCameraPos;
	SetLightDirection(cameraToPlayer.ToDxLib());

	// タイトルの画像の描画
	DrawRotaGraph(Game::kScreenWidth / 2, kTitleY, kTitleScale, m_titleBackGraphAngle, m_titleBackGraphHandle, true);
	DrawRotaGraph(Game::kScreenWidth / 2, kTitleY, kTitleScale, 0.0, m_titleGraphHandle, true);

	// テキストUIの描画
	// Aボタンが押されたあとだったら早く点滅
	if (m_pressStartFrameCount)
	{
		if (m_frameCount % 6 < 3)
		{
			std::wstring text = L"Aボタンでスタート";
			int textWidth = GetDrawFormatStringWidthToHandle(m_fontHandle, text.c_str());
			int x = Game::kScreenWidth / 2 - textWidth / 2;
			int y = Game::kScreenHeight / 2 + 50 / 2;
			DrawFormatStringToHandle(x, y, 0xffffff, m_fontHandle, text.c_str());
		}
	}
	else	// Aボタンが押されていなかったら普通に点滅
	{
		if (m_frameCount % 60 < 30)
		{
			std::wstring text = L"Aボタンでスタート";
			int textWidth = GetDrawFormatStringWidthToHandle(m_fontHandle, text.c_str());
			int x = Game::kScreenWidth / 2 - textWidth / 2;
			int y = Game::kScreenHeight / 2 + 50 / 2;
			DrawFormatStringToHandle(x, y, 0xffffff, m_fontHandle, text.c_str());
		}
	}
#ifdef _DEBUG
	DrawString(0, 0, L"SceneTitle\n1キーでシーンを終わる", 0xffffff);
#endif
}
