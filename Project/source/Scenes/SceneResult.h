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
	SceneResult(Input& input,SceneManager& sceneManager,int score);
	virtual ~SceneResult() override;

	virtual void Init() override;
	virtual void End() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	void DrawScoreText();
	void DrawSubText();

private:
	int m_frameCount = 0;
	// Aボタンが押されたあとのフレーム数
	int m_pressButtonFrameCount = 0;
	// スコアが表示完了したかどうか
	bool m_isDispScoreComplete = false;
	int m_dispScoreCompleteAfterFrame = 0;

	// フォントのハンドル
	int m_scoreFontHandle = -1;
	int m_fontHandle = -1;
	// スコアUIのY位置
	int m_scoreTextY = 0;

	// ランク画像のハンドル
	int m_rankGraphHandle = -1;
	// ランク画像の描画倍率
	float m_rankGraphScale = 0.0f;

	// ランク背景画像のハンドル
	int m_rankBackGraphHandle = -1;
	// ランク背景画像の角度
	float m_rankBackAngle = 0.0f;

	// 結果画面で表示するコイン
	std::vector<std::shared_ptr<ResultCoin>> m_pResultCoins;
	// 背景
	std::shared_ptr<SkyBox> m_pSkyBox = nullptr;;

	// スコア
	int m_score = 0;
	float m_dispScore = 0;	// 表示用のスコア(スコアが徐々に上がるようにするため)
};