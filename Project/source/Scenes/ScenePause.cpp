#include "ScenePause.h"
#include "DxLib.h"
#include "../Game.h"
#include "../System/Input.h"
#include "../Managers/SoundManager.h"

#include "SceneManager.h"
#include "SceneTitle.h"

ScenePause::ScenePause(Input& input, SceneManager& sceneManager):
	SceneBase(input, sceneManager)
{
}

ScenePause::~ScenePause()
{
}

void ScenePause::Init()
{
}

void ScenePause::End()
{
}

void ScenePause::Update()
{
	// スタートボタンが押されたらシーンを閉じ、一つ前のシーンに戻る
	if (m_input.IsTriggerd(XINPUT_BUTTON_START))
	{
		m_sceneManager.PopScene();
		return;
	}
	// セレクトボタンが押されたらシーンをすべて閉じ、タイトルシーンに戻る
	if (m_input.IsTriggerd(XINPUT_BUTTON_BACK))
	{
		SoundManager::GetInstance().StopSound(L"InGameBGM");
		m_sceneManager.ResetScene(std::make_shared<SceneTitle>(m_input, m_sceneManager));
		return;
	}
}

void ScenePause::Draw()
{
	// 画面全体を半透明の黒で塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	DrawString(0, 0, L"ScenePause\nStart :ゲームを再開\nSelect:タイトルに戻る", 0xffffff);
#endif
}