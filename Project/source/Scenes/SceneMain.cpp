#include "SceneMain.h"
#include "DxLib.h"
#include "../Game.h"
#include <cassert>
#include <format>

#include "../System/Input.h"

#include "../System/Camera.h"

#include "../Managers/ModelManager.h"
#include "../Managers/CollisionManager.h"
#include "../Managers/EnemyManager.h"
#include "../Managers/CoinManager.h"
#include "../Managers/ChestManager.h"
#include "../Managers/EffectManager.h"

#include "../GameObjects/Player.h"

namespace
{
	// 使用するモデルのファイル名と登録名
	const std::pair<std::wstring, std::wstring> kModelNames[4] = {
		{ L"data/Models/Player.MV1", L"Player" },
		{ L"data/Models/Enemy.MV1",  L"Enemy"  },
		{ L"data/Models/Chest.MV1",  L"Chest"  },
		{ L"data/Models/Coin.MV1",   L"Coin"   },
	};

	// フォントのサイズ
	constexpr int kStartFontSize = 100;
	constexpr int kUIFontSize = 50;

	// 地面の色
	constexpr unsigned int kGroundColor = 0x44cc44;

	// 敵がスポーンする間隔(フレーム)
	constexpr int kEnemySpawnInterval = 120;
	// 宝箱がスポーンする間隔(フレーム)
	constexpr int kChestSpawnInterval = 300;

	// 敵の存在上限数
	constexpr int kEnemyMaxNum = 10;
	// 宝箱の存在上限数
	constexpr int kChestMaxNum = 3;
}

SceneMain::SceneMain(Input& input) :
	m_input(input)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_startFontHandle = CreateFontToHandle(nullptr, kStartFontSize, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_startFontHandle != -1 && "フォントが正しく生成されませんでした");
	m_uiFontHandle = CreateFontToHandle(nullptr, kUIFontSize, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_uiFontHandle != -1 && "フォントが正しく生成されませんでした");

	// モデルマネージャーを生成しモデルのロードを依頼
	m_pModelManager = std::make_shared<ModelManager>();
	for (auto& names : kModelNames)
	{
		m_pModelManager->LoadModel(names.first, names.second);
	}
	// コリジョンマネージャーの生成
	m_pCollisionManager = std::make_shared<CollisionManager>();

	// エフェクトマネージャーの生成と初期化
	m_pEffectManager = std::make_shared<EffectManager>();
	m_pEffectManager->Init();
	m_pEffectManager->LoadEffect(L"data/Effects/Benediction.efk", L"Benediction", 20.0f);

	// カメラの生成と初期化
	m_pCamera = std::make_shared<Camera>(m_input);
	m_pCamera->Init();

	// コインマネージャーの生成と初期化
	m_pCoinManager = std::make_shared<CoinManager>(*m_pModelManager, *m_pCollisionManager);
	m_pCoinManager->Init();

	// プレイヤーの生成と初期化
	m_pPlayer = std::make_shared<Player>(m_input,*m_pCollisionManager);
	m_pPlayer->SetHandle(m_pModelManager->DuplicateModel(L"Player"));
	m_pPlayer->Init();
	m_pCollisionManager->Register(m_pPlayer);
	// 最初は操作不能にする
	m_pPlayer->SetCanControll(false);

	// 敵マネージャーの生成と初期化
	m_pEnemyManager = std::make_shared<EnemyManager>(*m_pModelManager, *m_pCollisionManager,*m_pCoinManager,*m_pEffectManager, *m_pPlayer);
	m_pEnemyManager->Init();

	// 宝箱マネージャーの生成と初期化
	m_pChestManager = std::make_shared<ChestManager>(*m_pModelManager,*m_pCollisionManager, *m_pCoinManager,*m_pEffectManager);
	m_pChestManager->Init();
}

void SceneMain::End()
{
	DeleteFontToHandle(m_startFontHandle);
	DeleteFontToHandle(m_uiFontHandle);

	// モデルマネージャーの終了処理(ロードしたモデルの削除)
	m_pModelManager->End();

	// プレイヤーの終了処理
	m_pPlayer->End();

	// 敵マネージャーの終了処理
	m_pEnemyManager->End();
	m_pCoinManager->End();
	m_pChestManager->End();
}

void SceneMain::Update()
{
	m_frameCount++;

	if (m_isStarted)
	{
		m_gameCount++;

		// 定期的に敵を召還する
		if (m_gameCount % kEnemySpawnInterval == 0)
		{
			// 今存在している敵の数が上限を超えていなければ召還
			if (m_pEnemyManager->GetEnemyNum() < kEnemyMaxNum)
			{
				m_pEnemyManager->Spawn();
			}
		}
		// 定期的に宝箱を召還する
		if (m_gameCount % kChestSpawnInterval == 0)
		{
			// 今存在している宝箱の数が上限を超えていなければ召還
			if (m_pChestManager->GetChestNum() < kChestMaxNum)
			{
				m_pChestManager->Spawn(m_pPlayer->GetPos());
			}
		}
	}

	// カメラの更新
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pPlayer->SetCameraAngleY(m_pCamera->GetAngleY());	// プレイヤーにカメラの角度を渡す

	// 各オブジェクトの更新
	m_pCamera->Update();
	m_pPlayer->Update();
	m_pEnemyManager->Update();
	m_pChestManager->Update();
	m_pCoinManager->Update();
	m_score = m_pCoinManager->GetCoinNum() * 100;

	// 当たり判定の更新
	m_pCollisionManager->Update();

	// カメラからプレイヤーへのベクトルを生成
	auto cameraToPlayer = m_pPlayer->GetPos() - m_pCamera->GetPos();
	// 生成したベクトルの方向のライトを設定
	SetLightDirection(cameraToPlayer.ToDxLib());

	// エフェクトマネージャーの更新
	m_pEffectManager->Update();
}

void SceneMain::Draw()
{

	// 各オブジェクトの描画
	m_pPlayer->Draw();
	m_pEnemyManager->Draw();
	m_pChestManager->Draw();
	m_pCoinManager->Draw();

	// 床の描画
	DrawTriangle3D({ -Game::kFieldSize,0,Game::kFieldSize }, { Game::kFieldSize,0,Game::kFieldSize }, { Game::kFieldSize,0,-Game::kFieldSize }, kGroundColor, true);
	DrawTriangle3D({ -Game::kFieldSize,0,Game::kFieldSize }, { Game::kFieldSize,0,-Game::kFieldSize }, { -Game::kFieldSize,0,-Game::kFieldSize }, kGroundColor, true);

	// エフェクトの描画
	m_pEffectManager->Draw();

	// ゲーム開始前のカウントダウンの描画
	DrawStart();

	float sec = 60.0f - static_cast<float>(m_gameCount) / 60.0f;
	std::wstring text = std::format(L"残り時間:{:.1f}", sec);
	auto strWidth = GetDrawFormatStringWidthToHandle(m_uiFontHandle, text.c_str());
	int x = Game::kScreenWidth / 2 - strWidth / 2;
	DrawFormatStringToHandle(x, 0, 0xffffff, m_uiFontHandle, text.c_str());

#ifdef _DEBUG
	DrawFormatString(0, 32, 0x000000, L"SCORE:%d", m_score);

	DrawGrid();
	DrawString(0,0,L"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"FRAME:%d", m_frameCount);
#endif
}

void SceneMain::DrawStart()
{
	m_startCount++;

	int second = m_startCount / 60;
	if (second > 4)
	{
		return;
	}

	if (m_startCount < 60)
	{
		return;
	}

	std::wstring text = L"temp";
	text = std::to_wstring(4 - second);

	if (second > 3)
	{
		text = L"START!";
		m_isStarted = true;
		m_pPlayer->SetCanControll(true);
	}

	auto stringWidth = GetDrawFormatStringWidthToHandle(m_startFontHandle, text.c_str());
	int x = Game::kScreenWidth / 2 - stringWidth / 2;
	int y = Game::kScreenHeight / 2 - kStartFontSize / 2;
	DrawFormatStringToHandle(x, y, 0xffffff, m_startFontHandle, text.c_str());
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
