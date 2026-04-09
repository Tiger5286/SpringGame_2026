#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

class SkyBox;
class ResultCoin;

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

	std::vector<std::shared_ptr<ResultCoin>> m_pResultCoins;
	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;;

	int m_score = 0;
};