#pragma once
#include <memory>

class ModelManager;

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void Update();
	void Draw();
private:
	void DrawGrid();

private:
	int m_frameCount;

	std::shared_ptr<ModelManager> m_pModelManager = nullptr;
};

