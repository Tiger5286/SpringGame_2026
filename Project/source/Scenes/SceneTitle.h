#pragma once
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle(Input& input);
	virtual ~SceneTitle();

	virtual void Init() override;
	virtual void End() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	int m_titleFontHandle = -1;
	int m_fontHandle = -1;
};

