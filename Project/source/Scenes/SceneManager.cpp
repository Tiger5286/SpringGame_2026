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

	auto firstScene = std::make_shared<SceneTitle>(*m_pInput);
	firstScene->Init();
	m_pScenes.push_back(firstScene);

	m_pFade = std::make_shared<Fade>();
	m_pFade->StartFadeIn();
}

void SceneManager::End()
{
	for (auto& scene : m_pScenes)
	{
		scene->End();
	}
}

void SceneManager::Update()
{
	// 前のフレームでシーンが終わっているかどうかを覚えておく
	m_isPrevSceneEnd = m_pScenes.back()->IsEnd();

	// 入力の更新
	m_pInput->Update();

	// フェードの更新
	m_pFade->Update();

	// フェード中はシーンの更新を行わない
	if (!m_pFade->IsFading())
	{
		// 一番後ろのシーンだけ更新する
		m_pScenes.back()->Update();
	}

	// シーンが終了していたら
	if (m_pScenes.back()->IsEnd())
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
			switch (m_pScenes.back()->GetSceneType())
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
	// 全てのシーンを描画する
	for (auto& scene : m_pScenes)
	{
		scene->Draw();
	}
	m_pFade->Draw();
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> newScene)
{
	// 全てのシーンの終了処理を行う
	for (auto& scene : m_pScenes)
	{
		scene->End();
	}
	// シーンのリストをクリアする
	m_pScenes.clear();

	// 新しいシーンの初期化処理を行う
	newScene->Init();
	m_pScenes.push_back(newScene);
}

void SceneManager::ChangeSceneMainToResult()
{
	// シーン遷移前にスコアを保存
	m_score = std::dynamic_pointer_cast<SceneMain>(m_pScenes.back())->GetScore();

	// 全てのシーンの終了処理を行う
	for (auto& scene : m_pScenes)
	{
		scene->End();
	}
	// シーンのリストをクリアする
	m_pScenes.clear();

	// リザルトシーンを生成
	auto resultScene = std::make_shared<SceneResult>(*m_pInput);
	// リザルトシーンにスコアを渡す
	resultScene->SetScore(m_score);
	// シーンを初期化
	resultScene->Init();

	// シーンをリストに追加
	m_pScenes.push_back(resultScene);
}
