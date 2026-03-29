#pragma once
#include <memory>

class SceneBase;

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
	std::shared_ptr<SceneBase> m_pCurrentScene = nullptr;
};