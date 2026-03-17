#pragma once
#include <memory>

class Input;

class ModelManager;

class Player;

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void End();
	void Update();
	void Draw();
private:
	void DrawGrid();

private:
	int m_frameCount = 0;

	std::shared_ptr<Input> m_pInput = nullptr;

	std::shared_ptr<ModelManager> m_pModelManager = nullptr;

	std::shared_ptr<Player> m_pPlayer = nullptr;
};

