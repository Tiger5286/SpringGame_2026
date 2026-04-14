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

	// 一番最後のシーンを新しいシーンに切り替える
	void ChangeScene(std::shared_ptr<SceneBase> newScene);
	// すべてのシーンを削除し、新しいシーンを追加する
	void ResetScene(std::shared_ptr<SceneBase> newScene);
	// リストの最後に新しいシーンを追加する
	void PushScene(std::shared_ptr<SceneBase> newScene);
	// リストの最後のシーンを終了し、削除する
	void PopScene();

private:
	// SceneMainからSceneResultに切り替える際、スコアを渡すために特別な関数を用意
	//void ChangeSceneMainToResult();

private:
	std::shared_ptr<Input> m_pInput = nullptr;

	std::shared_ptr<SceneBase> m_pNextScene = nullptr;
	std::list<std::shared_ptr<SceneBase>> m_pScenes;

	std::shared_ptr<Fade> m_pFade = nullptr;

	bool m_isRequestReset = false;

	int m_sceneChangeFrame = 0;

	int m_score = 0;
};