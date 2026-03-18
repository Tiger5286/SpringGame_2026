#pragma once
#include <memory>
#include "../System/Input.h"

class Camera;

class ModelManager;

class Player;
class Enemy;

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

	std::shared_ptr<Camera> m_pCamera = nullptr;

	std::shared_ptr<ModelManager> m_pModelManager = nullptr;

	std::shared_ptr<Player> m_pPlayer = nullptr;
	std::shared_ptr<Enemy> m_pEnemy = nullptr;
};

