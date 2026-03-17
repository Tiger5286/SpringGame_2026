#pragma once
#include <memory>
#include "../System/Input.h"

class ModelManager;

class Player;

class SceneMain
{
public:
	SceneMain(Input& input);
	~SceneMain();

	void Init();
	void End();
	void Update();
	void Draw();
private:
	void DrawGrid();

private:
	int m_frameCount = 0;
	Input& m_input;

	std::shared_ptr<ModelManager> m_pModelManager = nullptr;

	std::shared_ptr<Player> m_pPlayer = nullptr;
};

