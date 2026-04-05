#pragma once
#include "SceneBase.h"
#include <memory>

class SkyBox;

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

	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;;

	int m_score = 0;
};