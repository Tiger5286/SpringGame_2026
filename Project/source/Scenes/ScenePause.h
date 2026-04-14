#pragma once
#include "SceneBase.h"

class ScenePause:
	public SceneBase
{
public:
	ScenePause(Input& input, SceneManager& sceneManager);
	virtual ~ScenePause() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

private:
	int m_titleFontHandle = -1;
	int m_fontHandle = -1;
};