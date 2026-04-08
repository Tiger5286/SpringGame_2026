#pragma once
#include <memory>

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

private:
	void ChangeScene(std::shared_ptr<SceneBase> newScene);

private:
	std::shared_ptr<Input> m_pInput = nullptr;
	std::shared_ptr<SceneBase> m_pCurrentScene = nullptr;
	std::shared_ptr<Fade> m_pFade = nullptr;

	bool m_isPrevSceneEnd = false;

	int m_score = 0;
};