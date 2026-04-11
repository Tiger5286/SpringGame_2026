#include "SceneManager.h"
#include "../System/Input.h"
#include <cassert>
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneResult.h"
#include "../System/Fade.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	m_pInput = std::make_shared<Input>();

	m_pCurrentScene = std::make_shared<SceneTitle>(*m_pInput);
	m_pCurrentScene->Init();

	m_pFade = std::make_shared<Fade>();
	m_pFade->StartFadeIn();
}

void SceneManager::End()
{
	m_pCurrentScene->End();
}

void SceneManager::Update()
{
	// 前のフレームでシーンが終わっているかどうかを覚えておく
	m_isPrevSceneEnd = m_pCurrentScene->IsEnd();

	// 入力の更新
	m_pInput->Update();

	// フェードの更新
	m_pFade->Update();

	// フェード中はシーンの更新を行わない
	if (!m_pFade->IsFading())
	{
		m_pCurrentScene->Update();
	}

	// シーンが終了していたら
	if (m_pCurrentScene->IsEnd())
	{
		// シーンが終了した瞬間だけ
		if (!m_isPrevSceneEnd)
		{
			// フェードアウトを開始する
			m_pFade->StartFadeOut();
		}

		// フェードしていなければシーンを切り替える
		if (!m_pFade->IsFading())
		{
			// フェードインを開始
			m_pFade->StartFadeIn();
			// シーンを切り替え
			switch (m_pCurrentScene->GetSceneType())
			{
			case SceneType::Title:
				ChangeScene(std::make_shared<SceneMain>(*m_pInput));
				break;
			case SceneType::Main:
				ChangeSceneMainToResult();
				break;
			case SceneType::Result:
				ChangeScene(std::make_shared<SceneTitle>(*m_pInput));
				break;
			default:
				assert(false && "存在しないシーンタイプです");
				break;
			}
		}
	}
}

void SceneManager::Draw()
{
	m_pCurrentScene->Draw();
	m_pFade->Draw();
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> newScene)
{
	// 現在のシーンの終了処理を行う
	m_pCurrentScene->End();
	// 新しいシーンの初期化処理を行う
	newScene->Init();
	m_pCurrentScene = newScene;
}

void SceneManager::ChangeSceneMainToResult()
{
	// シーン遷移前にスコアを保存
	m_score = std::dynamic_pointer_cast<SceneMain>(m_pCurrentScene)->GetScore();

	// 現在のシーンの終了処理を行う
	m_pCurrentScene->End();
	// リザルトシーンを生成
	m_pCurrentScene = std::make_shared<SceneResult>(*m_pInput);
	// リザルトシーンにスコアを渡す
	std::dynamic_pointer_cast<SceneResult>(m_pCurrentScene)->SetScore(m_score);
	// シーンを初期化
	m_pCurrentScene->Init();
}
