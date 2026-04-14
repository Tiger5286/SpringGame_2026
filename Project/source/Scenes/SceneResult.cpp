#include "SceneResult.h"
#include "Dxlib.h"
#include <cassert>
#include <string>
#include <format>
#include "../System/Input.h"
#include "../Game.h"
#include "../System/SkyBox.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ModelManager.h"
#include "../System/ResultCoin.h"

#include "SceneManager.h"
#include "SceneTitle.h"

namespace
{
	const Vector3 kCameraPos = Vector3(0,0,-700);

	constexpr float kResultCoinAppearY = 2000.0f;
}

SceneResult::SceneResult(Input& input, SceneManager& sceneManager, int score):
	SceneBase(input,sceneManager),
	m_score(score)
{
	m_sceneType = SceneType::Result;
}

SceneResult::~SceneResult()
{
}

void SceneResult::Init()
{
	// フォントの生成
	m_fontHandle = CreateFontToHandle(Game::kFontName, 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_fontHandle != -1 && "フォントのハンドルの作成に失敗しました");
	m_scoreFontHandle = CreateFontToHandle(Game::kFontName, 70, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_scoreFontHandle != -1 && "フォントのハンドルの作成に失敗しました");

	// カメラの設定
	SetCameraPositionAndTarget_UpVecY(kCameraPos.ToDxLib(), VGet(0, 0, 0));
	// ライトの向きを設定
	SetLightDirection((Vector3(0, 0, 0) - kCameraPos).ToDxLib());

	int coinNum = m_score / 500 + 1;
	// ゲームシーンでコインを1枚も取っていなかったら表示するコインも0にする
	if (m_score == 0)
	{
		coinNum = 0;
	}

	m_pResultCoins.resize(coinNum);
	for (int i = 0; i < m_pResultCoins.size(); i++)
	{
		m_pResultCoins[i] = std::make_shared<ResultCoin>();
		m_pResultCoins[i]->Init(ModelManager::GetInstance().DuplicateModel(L"Coin"));
		m_pResultCoins[i]->Spawn();
		// コインのY位置をそれぞれずらして配置することで継続的に降ってきてるように見せる
		m_pResultCoins[i]->m_pos.y += kResultCoinAppearY / coinNum * i;
	}

	m_pSkyBox = std::make_shared<SkyBox>();
	m_pSkyBox->Init();
	m_pSkyBox->SetCameraPos(kCameraPos);

	// BGMの再生
	SoundManager::GetInstance().PlaySoundGame(L"ResultBGM", true, true);
}

void SceneResult::End()
{
	DeleteFontToHandle(m_fontHandle);

	m_pSkyBox->End();
}

void SceneResult::Update()
{
	m_frameCount++;
	// Aボタンが押されたあとのフレーム数をカウント
	if (m_pressStartFrameCount)
	{
		m_pressStartFrameCount++;
	}
	// Aボタンが押されてから一定のフレームが経過したらシーンを終わる
	if (m_pressStartFrameCount > 40)
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_input, m_sceneManager));
		return;
	}

	// コインを更新
	for (auto& pCoin : m_pResultCoins)
	{
		pCoin->Update();
	}

	m_pSkyBox->Update();

	// Aボタンが押されたらシーンを終わる
	if (m_input.IsTriggerd(XINPUT_BUTTON_A))
	{
		// スコアの表示が完了していたらシーンを終わる準備をする
		if (m_isDispScoreComplete)
		{
			// Aボタンが押されたあとのフレーム数をカウント開始する
			if (!m_pressStartFrameCount)
			{
				m_pressStartFrameCount = 1;
			}
			SoundManager::GetInstance().PlaySoundGame(L"Decision");
			SoundManager::GetInstance().StopSound(L"ResultBGM", true);
		}
		else	// スコアの表示が完了していなかったらスコアを完全に表示する
		{
			m_dispScore = m_score;
			m_isDispScoreComplete = true;
		}
	}

	// スコアを徐々に上げる
	m_dispScore = std::lerp(m_dispScore, m_score, 0.05f);
	// スコアがほぼ等しくなったら完全に等しくする
	if (m_score - m_dispScore < 0.05f)
	{
		m_dispScore = m_score;
		m_isDispScoreComplete = true;
	}

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_1))
	{
		SoundManager::GetInstance().StopSound(L"ResultBGM", true);
		m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_input, m_sceneManager));
		return;
	}
#endif
}

void SceneResult::Draw()
{
	m_pSkyBox->Draw();

	for (auto& pCoin : m_pResultCoins)
	{
		pCoin->Draw();
	}

	// スコアを描画
	DrawScoreText();

	// サブテキストを描画
	if (m_pressStartFrameCount)
	{	// Aボタンが押されたあとはサブテキストを早く点滅させる
		if (m_frameCount % 6 < 3)
		{
			DrawSubText();
		}
	}
	else
	{	// Aボタンが押されるまではサブテキストをゆっくり点滅させる
		if (m_frameCount % 60 < 30)
		{
			DrawSubText();
		}
	}

#ifdef _DEBUG
	DrawString(0, 0, L"SceneResult\n1キーでシーンを終わる", 0xffffff);
	DrawFormatString(100, 100, 0xffffff, L"%f", m_dispScore);
#endif
}

void SceneResult::DrawScoreText()
{
	// テキストを準備
	std::wstring resultText = std::format(L"スコア:{:d}", m_score);
	std::wstring dispResultText = std::format(L"スコア:{:d}", static_cast<int>(m_dispScore));
	// テキストの幅を取得
	int resultTextWidth = GetDrawStringWidthToHandle(resultText.c_str(), resultText.size(), m_scoreFontHandle);	// スコアのテキストの幅(表示用ではない方のスコアの幅)
	// スコアのテキストを描画
	int x = Game::kScreenWidth / 2 - resultTextWidth / 2;
	int y = Game::kScreenHeight / 2 - 70 / 2;
	DrawStringToHandle(x, y, dispResultText.c_str(), 0xffffff, m_scoreFontHandle);	// 表示用のスコアのテキストを描画
}

void SceneResult::DrawSubText()
{
	std::wstring subText = L"Aボタンでタイトルに戻る";
	int subTextWidth = GetDrawStringWidthToHandle(subText.c_str(), subText.size(), m_fontHandle);
	int x = Game::kScreenWidth / 2 - subTextWidth / 2;
	int y = Game::kScreenHeight / 4 * 3 - 50 / 2;
	DrawStringToHandle(x, y, subText.c_str(), 0xffffff, m_fontHandle);
}
