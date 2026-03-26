#include "SceneMain.h"
#include "DxLib.h"
#include "../Game.h"

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
	// モデルマネージャーを生成しモデルのロードを依頼
	m_pModelManager = std::make_shared<ModelManager>();
	for (auto& names : kModelNames)
	{
		m_pModelManager->LoadModel(names.first, names.second);
	}
	// コリジョンマネージャーの生成
	m_pCollisionManager = std::make_shared<CollisionManager>();

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

	// 敵マネージャーの生成と初期化
	m_pEnemyManager = std::make_shared<EnemyManager>(*m_pModelManager, *m_pCollisionManager,*m_pCoinManager, *m_pPlayer);
	m_pEnemyManager->Init();
	// 敵を生成
	m_pEnemyManager->Spawn();

	// 宝箱マネージャーの生成と初期化
	m_pChestManager = std::make_shared<ChestManager>(*m_pModelManager,*m_pCollisionManager, *m_pCoinManager);
	m_pChestManager->Init();

	// エフェクトマネージャーの生成と初期化
	m_pEffectManager = std::make_shared<EffectManager>();
	m_pEffectManager->Init();
	m_pEffectManager->LoadEffect(L"data/Effects/Benediction.efk", L"Benediction",20.0f);
	m_pEffectManager->PlayEffect(L"Benediction", Vector3(0.0f, 100.0f, 0.0f));
}

void SceneMain::End()
{
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

	// 定期的に敵を召還する
	if (m_frameCount % kEnemySpawnInterval == 0)
	{
		// 今存在している敵の数が上限を超えていなければ召還
		if (m_pEnemyManager->GetEnemyNum() < kEnemyMaxNum)
		{
			m_pEnemyManager->Spawn();
		}
	}
	// 定期的に宝箱を召還する
	if (m_frameCount % kChestSpawnInterval == 0)
	{
		// 今存在している宝箱の数が上限を超えていなければ召還
		if (m_pChestManager->GetChestNum() < kChestMaxNum)
		{
			m_pChestManager->Spawn(m_pPlayer->GetPos());
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

	// エフェクトの描画
	m_pEffectManager->Draw();

	// 床の描画
	DrawTriangle3D({ -Game::kFieldSize,0,Game::kFieldSize }, { Game::kFieldSize,0,Game::kFieldSize }, { Game::kFieldSize,0,-Game::kFieldSize }, kGroundColor, true);
	DrawTriangle3D({ -Game::kFieldSize,0,Game::kFieldSize }, { Game::kFieldSize,0,-Game::kFieldSize }, { -Game::kFieldSize,0,-Game::kFieldSize }, kGroundColor, true);
#ifdef _DEBUG
	DrawGrid();
	DrawString(0,0,L"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"FRAME:%d", m_frameCount);
#endif
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
