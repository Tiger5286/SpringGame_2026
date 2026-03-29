#pragma once
#include <memory>

class SceneBase;
class Input;

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
};