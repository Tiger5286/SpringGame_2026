#include "ScenePause.h"
#include "DxLib.h"
#include "../Game.h"
#include "../System/Input.h"
#include "../Managers/SoundManager.h"
#include <cassert>

#include "SceneManager.h"
#include "SceneTitle.h"

namespace
{
	// フォントのサイズ
	constexpr int kTitleFontSize = 70;
	constexpr int kFontSize = 50;

	// UIのY位置
	constexpr int kTitleTextY = 100;
	constexpr int kResumeTextY = 500;
	constexpr int kReturnTitleTextY = 600;
}

ScenePause::ScenePause(Input& input, SceneManager& sceneManager):
	SceneBase(input, sceneManager)
{
}

ScenePause::~ScenePause()
{
}

void ScenePause::Init()
{
	// フォントハンドルを作成
	m_titleFontHandle = CreateFontToHandle(Game::kFontName, kTitleFontSize, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	m_fontHandle = CreateFontToHandle(Game::kFontName, kFontSize, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_titleFontHandle != -1 && "フォントが正しく生成できませんでした");
	assert(m_fontHandle != -1 && "フォントが正しく生成できませんでした");
}

void ScenePause::End()
{
	// フォントハンドルを削除
	DeleteFontToHandle(m_titleFontHandle);
	DeleteFontToHandle(m_fontHandle);
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

	// テキストを準備
	std::wstring titleText = L"ポーズ";
	std::wstring resumeText = L"START:ゲームを再開";
	std::wstring returnTitleText = L"BACK:タイトルに戻る";
	// テキストの幅を取得
	int titleWidth = GetDrawStringWidthToHandle(titleText.c_str(), titleText.size(), m_titleFontHandle);
	int resumeWidth = GetDrawStringWidthToHandle(resumeText.c_str(), resumeText.size(), m_fontHandle);
	int returnTitleWidth = GetDrawStringWidthToHandle(returnTitleText.c_str(), returnTitleText.size(), m_fontHandle);
	// テキストを描画
	DrawStringToHandle((Game::kScreenWidth - titleWidth) / 2, kTitleTextY, titleText.c_str(), 0xffffff, m_titleFontHandle);
	DrawStringToHandle((Game::kScreenWidth - resumeWidth) / 2, kResumeTextY, resumeText.c_str(), 0xffffff, m_fontHandle);
	DrawStringToHandle((Game::kScreenWidth - returnTitleWidth) / 2, kReturnTitleTextY, returnTitleText.c_str(), 0xffffff, m_fontHandle);

#ifdef _DEBUG
	DrawString(0, 0, L"ScenePause\nStart :ゲームを再開\nBack:タイトルに戻る", 0xffffff);
#endif
}