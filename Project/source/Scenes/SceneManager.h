#pragma once
#include <memory>
#include <list>

class SceneBase;
class Input;
class Fade;

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	void Init();
	void End();
	void Update();
	void Draw();

	// リストの全てのシーンを終了し、新しいシーンを開始する
	void ChangeScene(std::shared_ptr<SceneBase> newScene);

private:
	// SceneMainからSceneResultに切り替える際、スコアを渡すために特別な関数を用意
	//void ChangeSceneMainToResult();

private:
	std::shared_ptr<Input> m_pInput = nullptr;

	std::shared_ptr<SceneBase> m_pNextScene = nullptr;
	std::list<std::shared_ptr<SceneBase>> m_pScenes;

	std::shared_ptr<Fade> m_pFade = nullptr;

	int m_sceneChangeFrame = 0;

	int m_score = 0;
};