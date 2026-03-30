#pragma once
#include "SceneBase.h"
class SceneResult :
    public SceneBase
{
public:
	SceneResult(Input& input);
	virtual ~SceneResult() override;

	virtual void Init() override;
	virtual void End() override;
	virtual void Update() override;
	virtual void Draw() override;

	void SetScore(int score) { m_score = score; }

private:
	int m_fontHandle = -1;

	int m_score = 0;
};

