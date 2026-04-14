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

#include "../System/SkyBox.h"

#include "../Managers/SoundManager.h"

namespace
{
	// エフェクトのデータ
	struct EffectData
	{
		const std::wstring filePath;	// エフェクトのファイルパス
		const std::wstring key;			// エフェクトの登録名
		const float scale;				// エフェクトの拡大率
	};
	// 使用するエフェクトのファイルパスと登録名と拡大率
	const EffectData kEffectData[] =
	{
		{L"data/Effects/Benediction.efk", L"Benediction", 20.0f  },
		{L"data/Effects/Hit.efkefc",L"Hit",100.0f},
		{L"data/Effects/fire.efkefc",L"Explosion",100.0f}
	};

	// フォントのサイズ
	constexpr int kTitleFontSize = 100;
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

	// ゲームの制限時間(秒)
	constexpr int kGameTimeLimit = 60;

	// カメラの初期位置
	const Vector3 kCameraFirstPos = Vector3(0, 400, -1000);
}

SceneMain::SceneMain(Input& input) :
	SceneBase(input)
{
	m_sceneType = SceneType::Main;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// フォントの生成
	m_titleFontHandle = CreateFontToHandle(Game::kFontName, kTitleFontSize, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_titleFontHandle != -1 && "フォントが正しく生成されませんでした");
	m_uiFontHandle = CreateFontToHandle(Game::kFontName, kUIFontSize, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(m_uiFontHandle != -1 && "フォントが正しく生成されませんでした");

	// 画像のロード
	m_howToPlayGraphHandle = LoadGraph(L"data/Graphs/howToPlay.png");
	assert(m_howToPlayGraphHandle != -1 && "画像が正しくロードされませんでした");

	// コリジョンマネージャーの生成
	m_pCollisionManager = std::make_shared<CollisionManager>();

	// エフェクトマネージャーの生成と初期化
	m_pEffectManager = std::make_shared<EffectManager>();
	m_pEffectManager->Init();
	// エフェクトのロード
	for (auto& data : kEffectData)
	{
		m_pEffectManager->LoadEffect(data.filePath, data.key, data.scale);
	}

	// カメラの生成と初期化
	m_pCamera = std::make_shared<Camera>(m_input);
	m_pCamera->Init();
	m_pCamera->SetPos(kCameraFirstPos);

	// コインマネージャーの生成と初期化
	m_pCoinManager = std::make_shared<CoinManager>(*m_pCollisionManager);
	m_pCoinManager->Init();

	// プレイヤーの生成と初期化
	m_pPlayer = std::make_shared<Player>(m_input,m_pCollisionManager,m_pEffectManager);
	m_pPlayer->SetHandle(ModelManager::GetInstance().DuplicateModel(L"Player"));
	m_pPlayer->Init();
	m_pCollisionManager->Register(m_pPlayer);
	// 最初は操作不能にする
	m_pPlayer->SetCanControll(false);

	// カメラの更新
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pPlayer->SetCameraAngleY(m_pCamera->GetAngleY());	// プレイヤーにカメラの角度を渡す
	m_pCamera->Update();

	// 敵マネージャーの生成と初期化
	m_pEnemyManager = std::make_shared<EnemyManager>(*m_pCollisionManager,*m_pCoinManager,*m_pEffectManager, *m_pPlayer);
	m_pEnemyManager->Init();

	// 宝箱マネージャーの生成と初期化
	m_pChestManager = std::make_shared<ChestManager>(*m_pCollisionManager, *m_pCoinManager,*m_pEffectManager);
	m_pChestManager->Init();

	// スカイボックスの生成
	m_pSkyBox = std::make_shared<SkyBox>();
	m_pSkyBox->SetCameraPos(m_pCamera->GetPos());
	m_pSkyBox->Init();

	// シャドウマップの生成
	m_shadowMapHandle = MakeShadowMap(8192, 8192);

	// BGMの再生
	SoundManager::GetInstance().PlaySoundGame(L"InGameBGM", true, true);
}

void SceneMain::End()
{
	// フォントの削除
	DeleteFontToHandle(m_titleFontHandle);
	DeleteFontToHandle(m_uiFontHandle);

	// 画像の削除
	DeleteGraph(m_howToPlayGraphHandle);

	// プレイヤーの終了処理
	m_pPlayer->End();

	// 敵マネージャーの終了処理
	m_pEnemyManager->End();
	m_pCoinManager->End();
	m_pChestManager->End();

	m_pSkyBox->End();

	// シャドウマップを削除
	DeleteShadowMap(m_shadowMapHandle);
}

void SceneMain::Update()
{
	m_frameCount++;
	UpdateStart();

	// カウントダウン開始のタイミングでカウントダウンの音を鳴らす
	if (m_startCount == 60)
	{
		SoundManager::GetInstance().PlaySoundGame(L"CountDown");
	}

	// ゲーム開始後の処理
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

	// スコアの更新
	m_score = m_pCoinManager->GetCoinNum() * 100;

	// 当たり判定の更新
	m_pCollisionManager->Update();

	// カメラからプレイヤーへのベクトルを生成
	auto cameraToPlayer = m_pPlayer->GetPos() - m_pCamera->GetPos();
	// 生成したベクトルの方向のライトを設定
	SetLightDirection(cameraToPlayer.ToDxLib());

	// エフェクトマネージャーの更新
	m_pEffectManager->Update();

	// スカイボックスの更新
	m_pSkyBox->SetCameraPos(m_pCamera->GetPos());
	m_pSkyBox->Update();

	// ゲームの制限時間を超えたらゲーム終了
	if (m_gameCount > kGameTimeLimit * 60)
	{
		m_finishCount++;
		m_pPlayer->SetCanControll(false);
		SoundManager::GetInstance().StopSound(L"InGameBGM", true);
	}
	// 時間切れのタイミングでフィニッシュの音を鳴らす
	if (m_finishCount == 1)
	{
		SoundManager::GetInstance().PlaySoundGame(L"Finish");
		m_pPlayer->SetInvincible(true);
	}

	// ゲーム終了後、一定時間が経ったらシーンを終了
	if (m_finishCount > 120)
	{
		m_isEnd = true;
		SoundManager::GetInstance().StopSound(L"InGameBGM", true);
	}

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_1))
	{
		m_isEnd = true;
		SoundManager::GetInstance().StopSound(L"InGameBGM", true);
	}
	if (CheckHitKey(KEY_INPUT_2))
	{
		m_gameCount = kGameTimeLimit * (60 - 1);
	}
#endif
}

void SceneMain::Draw()
{
	// スカイボックスの描画
	m_pSkyBox->Draw();

	// シャドウマップへの描画開始
	ShadowMap_DrawSetup(m_shadowMapHandle);

	// シャドウマップへ各オブジェクトの描画
	m_pPlayer->Draw();
	m_pEnemyManager->Draw();
	m_pChestManager->Draw();
	m_pCoinManager->Draw();

	// シャドウマップへの描画終了
	ShadowMap_DrawEnd();

	// 各オブジェクトの描画
	m_pPlayer->Draw();
	m_pEnemyManager->Draw();
	m_pChestManager->Draw();
	m_pCoinManager->Draw();

	// シャドウマップを使用して描画開始
	SetUseShadowMap(0, m_shadowMapHandle);

	// 床用のライトの向きを生成
	auto temp = Vector3(0.0f, -1.5f, 1.0f);
	temp = Matrix4x4::GetRotYMatrix(m_pCamera->GetAngleY()) * temp;
	SetLightDirection(temp.ToDxLib());

	// 床の描画
	MV1DrawModel(ModelManager::GetInstance().GetModelHandle(L"Floor"));

	// 通常のライトの向きを設定
	auto cameraToPlayer = m_pPlayer->GetPos() - m_pCamera->GetPos();
	SetLightDirection(cameraToPlayer.ToDxLib());

	// シャドウマップを使用した描画終了
	SetUseShadowMap(0, -1);

	// エフェクトの描画
	m_pEffectManager->Draw();

	// 操作説明の画像の描画
	DrawGraph(0, Game::kScreenHeight - 150, m_howToPlayGraphHandle, true);

	// ゲーム開始前のカウントダウンの描画
	DrawStart();

	// ゲーム終了後のテキストの描画
	if (m_finishCount > 0)
	{
		DrawFinish();
	}

	// 残り時間の描画
	unsigned int color = 0xffffff;	// 最初は白で描画
	float sec = kGameTimeLimit - static_cast<float>(m_gameCount) / 60.0f;
	if (sec <= 10.0f) color = 0xff0000;	// 残り時間が10秒を切ったら赤で描画
	if (sec < 0) sec = 0.0f;
	std::wstring text = std::format(L"残り時間:{:.1f}", sec);
	auto strWidth = GetDrawFormatStringWidthToHandle(m_uiFontHandle, L"残り時間:60.0");
	int x = Game::kScreenWidth / 2 - strWidth / 2;
	DrawFormatStringToHandle(x, 20, color, m_uiFontHandle, text.c_str());

	// スコアの描画
	// 表示用スコアの更新
	if (m_dispScore < m_score) m_dispScore += 13;
	if (m_dispScore > m_score) m_dispScore = m_score;
	// テキストを描画
	text = std::format(L"スコア:{:d}", m_dispScore);
	DrawFormatStringToHandle(x, kUIFontSize + 20, 0xffffff, m_uiFontHandle, text.c_str());

#ifdef _DEBUG
	DrawGrid();
	DrawString(0,0,L"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"FRAME:%d", m_frameCount);
	DrawString(0, 32, L"1キーでシーンを終わる\n2キーで制限時間を1にする", 0xffffff);
	m_pCamera->GetPos().Draw(0, 75);
#endif
}

void SceneMain::UpdateStart()
{
	m_startCount++;
}

void SceneMain::DrawStart()
{
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

	auto stringWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, text.c_str());
	int x = Game::kScreenWidth / 2 - stringWidth / 2;
	int y = Game::kScreenHeight / 2 - kTitleFontSize / 2;
	DrawFormatStringToHandle(x, y, 0xffffff, m_titleFontHandle, text.c_str());
}

void SceneMain::DrawFinish()
{
	std::wstring text = L"Finish!";
	auto stringWidth = GetDrawFormatStringWidthToHandle(m_titleFontHandle, text.c_str());
	int x = Game::kScreenWidth / 2 - stringWidth / 2;
	int y = Game::kScreenHeight / 2 - kTitleFontSize / 2;
	DrawFormatStringToHandle(x, y, 0xffffff, m_titleFontHandle, text.c_str());
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
