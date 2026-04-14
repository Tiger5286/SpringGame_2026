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

	// スコアを受け取る(Initの前に実行する)
	void SetScore(int score) { m_score = score; }

private:
	// フォントのハンドル
	int m_fontHandle = -1;

	// 結果画面で表示するコイン
	std::vector<std::shared_ptr<ResultCoin>> m_pResultCoins;
	// 背景
	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;;

	// スコア
	int m_score = 0;
	float m_dispScore = 0;	// 表示用のスコア(スコアが徐々に上がるようにするため)
};