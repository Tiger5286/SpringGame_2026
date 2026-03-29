#include "SceneManager.h"
#include "../System/Input.h"
#include <cassert>
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneResult.h"

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
}

void SceneManager::End()
{
	m_pCurrentScene->End();
}

void SceneManager::Update()
{
	m_pInput->Update();
	m_pCurrentScene->Update();
	if (m_pCurrentScene->IsEnd())
	{
		switch (m_pCurrentScene->GetSceneType())
		{
			case SceneType::Title:
				ChangeScene(std::make_shared<SceneMain>(*m_pInput));
				break;
			case SceneType::Main:
				ChangeScene(std::make_shared<SceneResult>(*m_pInput));
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

void SceneManager::Draw()
{
	m_pCurrentScene->Draw();
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> newScene)
{
	// 現在のシーンの終了処理を行う
	m_pCurrentScene->End();
	// 新しいシーンの初期化処理を行う
	newScene->Init();
	m_pCurrentScene = newScene;
}
