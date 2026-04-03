#include "SceneTitle.h"
#include "Dxlib.h"
#include "../System/Input.h"
#include <string>
#include "../Game.h"
#include <cassert>
#include "../System/SkyBox.h"
#include "../System/Camera.h"

SceneTitle::SceneTitle(Input& input):
	SceneBase(input)
{
	m_sceneType = SceneType::Title;
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	m_titleFontHandle = CreateFontToHandle(nullptr, 100, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_titleFontHandle != -1 && "フォントが正しく生成されませんでした");
	m_fontHandle = CreateFontToHandle(nullptr, 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_fontHandle != -1 && "フォントが正しく生成されませんでした");

	m_pSkyBox = std::make_shared<SkyBox>();
	m_pSkyBox->Init();
}

void SceneTitle::End()
{
	DeleteFontToHandle(m_titleFontHandle);
	DeleteFontToHandle(m_fontHandle);

	m_pSkyBox->End();
}

void SceneTitle::Update()
{
	if (m_input.IsTriggerd(XINPUT_BUTTON_A))
	{
		m_isEnd = true;
	}

	m_pSkyBox->Update();

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_1))
	{
		m_isEnd = true;
	}
#endif
}

void SceneTitle::Draw()
{
	m_pSkyBox->Draw();

	std::wstring titleText = L"コインラッシュ！";
	std::wstring subText = L"Aボタンでスタート";
	int titleTextWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, titleText.c_str());
	int subTextWidth = GetDrawFormatStringWidthToHandle(m_fontHandle, subText.c_str());

	int x = Game::kScreenWidth / 2 - titleTextWidth / 2;
	int y = Game::kScreenHeight / 2 - 100 / 2;
	DrawFormatStringToHandle(x, y, 0xffffff, m_titleFontHandle, titleText.c_str());
	x = Game::kScreenWidth / 2 - subTextWidth / 2;
	y = Game::kScreenHeight / 2 + 100 / 2;
	DrawFormatStringToHandle(x, y, 0xffffff, m_fontHandle, subText.c_str());


#ifdef _DEBUG
	DrawString(0, 0, L"SceneTitle\n1キーでシーンを終わる", 0xffffff);
#endif
}
