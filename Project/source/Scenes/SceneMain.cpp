#include "SceneMain.h"
#include "DxLib.h"

#include "../Managers/ModelManager.h"

#include "../GameObjects/Player.h"

namespace
{
	std::pair<std::wstring, std::wstring> kModelNames[4] = {
		{L"data/Player.MV1",L"Player"},
		{L"data/Enemy.MV1",L"Enemy"},
		{L"data/Chest.MV1",L"Chest"},
		{L"data/Coin.MV1",L"Coin"},
	};
}

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// カメラの設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 300.0f, -700.0f), VGet(0.0f, 0.0f, 0.0f));
	SetupCamera_Perspective(DX_PI_F / 3.0f);
	SetCameraNearFar(200.0f, 1500.0f);

	// モデルのロードをモデルマネージャーに依頼
	m_pModelManager = std::make_shared<ModelManager>();
	for (auto& names : kModelNames)
	{
		m_pModelManager->LoadModel(names.first, names.second);
	}

	// プレイヤーの生成
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->SetHandle(m_pModelManager->DuplicateModel(L"Player"));
	m_pPlayer->Init();
}

void SceneMain::End()
{
	// モデルマネージャーの終了処理(ロードしたモデルの削除)
	m_pModelManager->End();

	// プレイヤーの終了処理
	m_pPlayer->End();
}

void SceneMain::Update()
{
	m_frameCount++;

	m_pPlayer->Update();
}

void SceneMain::Draw()
{
	m_pPlayer->Draw();

	DrawGrid();
	DrawString(0,0,L"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"FRAME:%d", m_frameCount);
}

void SceneMain::DrawGrid()
{
	// 直線の始点と終点
	VECTOR startPos;
	VECTOR endPos;

	for (int z = -300; z <= 300; z += 100)
	{
		startPos = VGet(-300.0f, 0.0f, static_cast<float>(z));
		endPos = VGet(300.0f, 0.0f, static_cast<float>(z));
		DrawLine3D(startPos, endPos, 0xff0000);
	}
	for (int x = -300; x <= 300; x += 100)
	{
		startPos = VGet(static_cast<float>(x), 0.0f, -300.0f);
		endPos = VGet(static_cast<float>(x), 0.0f, 300.0f);
		DrawLine3D(startPos, endPos, 0x0000ff);
	}
}
