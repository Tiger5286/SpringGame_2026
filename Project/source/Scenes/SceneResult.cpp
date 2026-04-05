#include "SceneResult.h"
#include "Dxlib.h"
#include <cassert>
#include <string>
#include <format>
#include "../System/Input.h"
#include "../Game.h"
#include "../System/SkyBox.h"

SceneResult::SceneResult(Input& input):
	SceneBase(input)
{
	m_sceneType = SceneType::Result;
}

SceneResult::~SceneResult()
{
}

void SceneResult::Init()
{
	m_fontHandle = CreateFontToHandle(nullptr, 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_fontHandle != -1 && "フォントのハンドルの作成に失敗しました");

	SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -100), VGet(0, 0, 0));

	m_pSkyBox = std::make_shared<SkyBox>();
	m_pSkyBox->Init();
	m_pSkyBox->SetCameraPos(Vector3(0,0,-100));
}

void SceneResult::End()
{
	DeleteFontToHandle(m_fontHandle);

	m_pSkyBox->End();
}

void SceneResult::Update()
{
	m_pSkyBox->Update();

	if (m_input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_isEnd = true;
	}

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_1))
	{
		m_isEnd = true;
	}
#endif
}

void SceneResult::Draw()
{
	m_pSkyBox->Draw();

	std::wstring resultText = std::format(L"スコア:{:d}", m_score);
	std::wstring subText = L"Aボタンでタイトルに戻る";

	int resultTextWidth = GetDrawStringWidthToHandle(resultText.c_str(), resultText.size(), m_fontHandle);
	int subTextWidth = GetDrawStringWidthToHandle(subText.c_str(), subText.size(), m_fontHandle);

	int x = Game::kScreenWidth / 2 - resultTextWidth / 2;
	int y = Game::kScreenHeight / 2 - 50 / 2;
	DrawStringToHandle(x, y, resultText.c_str(), 0xffffff, m_fontHandle);
	x = Game::kScreenWidth / 2 - subTextWidth / 2;
	y = Game::kScreenHeight / 2 + 50 / 2;
	DrawStringToHandle(x, y, subText.c_str(), 0xffffff, m_fontHandle);


#ifdef _DEBUG
	DrawString(0, 0, L"SceneResult\n1キーでシーンを終わる", 0xffffff);
#endif
}
