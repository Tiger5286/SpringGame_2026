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
#include "../System/Magnet.h"

#include "../System/SkyBox.h"

#include "../Managers/SoundManager.h"

#include "SceneManager.h"
#include "SceneResult.h"
#include "ScenePause.h"

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
		{L"data/Effects/fire.efkefc",L"Explosion",100.0f},
		{L"data/Effects/MagnetActive.efkefc",L"MagnetActive",100.0f},
		{L"data/Effects/MagnetRecharge.efkefc",L"MagnetRecharge",300.0f}
	};

	// フォントのサイズ
	constexpr int kTitleFontSize = 150;
	constexpr int kUIFontSize = 50;

	// シャドウマップのサイズ
	constexpr int kShadowMapSize = 8192;

	// コイン1枚あたりのスコア
	constexpr int kScorePerCoin = 100;

	// 敵がスポーンする間隔(フレーム)
	constexpr int kEnemySpawnInterval = Game::kFPS * 2;
	// 宝箱がスポーンする間隔(フレーム)
	constexpr int kChestSpawnInterval = Game::kFPS * 5;

	// 敵の存在上限数
	constexpr int kEnemyMaxNum = 10;
	// 宝箱の存在上限数
	constexpr int kChestMaxNum = 3;

	// ゲームの制限時間(秒)
	constexpr int kGameTimeLimit = 60;
	// 残り時間が少なくなった時に赤になる時間(秒)
	constexpr int kTimeLimitToRed = 10.0f;
	// 残り時間UIのY位置
	constexpr int kTimeY = 20;

	// 表示用スコアが実際のスコアに追いつくまでの増加速度
	constexpr int kScoreIncreaseSpeed = 13;
	// スコア増加を加速させるかどうかのスコア差
	constexpr int kScoreDiffToAccelerate = 1000;
	// スコア増加が加速しているときの増加量
	constexpr int kAcceleratedScoreIncreaseSpeed = 100;

	// ゲーム終了後、シーンを切り替えるまでの時間(フレーム)
	constexpr int kFinishInterval = Game::kFPS * 2;



	// カメラの初期位置
	const Vector3 kCameraFirstPos = Vector3(0, 400, -1000);

	// 床用のライトの向き
	const Vector3 kFloorLightDirection = Vector3(0.0f, -1.5f, 1.0f);

	// 操作説明画像の位置
	constexpr int kHowToPlayGraphX = 0;
	constexpr int kHowToPlayGraphY = Game::kScreenHeight - 150;
}

SceneMain::SceneMain(Input& input, SceneManager& sceneManager) :
	SceneBase(input,sceneManager)
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

	// マグネットの生成と初期化
	m_pMagnet = std::make_shared<Magnet>(m_input,*m_pPlayer,*m_pCoinManager,*m_pEffectManager);
	m_pMagnet->Init();

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
	m_shadowMapHandle = MakeShadowMap(kShadowMapSize, kShadowMapSize);

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

	m_pMagnet->End();

	// 敵マネージャーの終了処理
	m_pEnemyManager->End();
	m_pCoinManager->End();
	m_pChestManager->End();

	m_pSkyBox->End();

	// シャドウマップを削除
	DeleteShadowMap(m_shadowMapHandle);

	// エフェクトマネージャーの終了処理
	m_pEffectManager->StopEffectAll();
	m_pEffectManager->End();
}

void SceneMain::Update()
{
	m_frameCount++;
	UpdateStart();

	// スタートボタンでポーズする
	if (m_input.IsTriggerd(XINPUT_BUTTON_START))
	{	// ボタンが押された瞬間ポーズにする
		m_isPause = true;
		SoundManager::GetInstance().PlaySoundGame(L"Pause");
		m_sceneManager.PushScene(std::make_shared<ScenePause>(m_input, m_sceneManager));
		return;
	}
	else
	{	// 押されていければポーズではないと判断する
		m_isPause = false;
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

	m_pMagnet->Update();

	m_pEnemyManager->Update();
	m_pChestManager->Update();

	m_pCoinManager->SetPlayerPos(m_pPlayer->GetPos());
	m_pCoinManager->Update();

	// スコアの更新
	m_score = m_pCoinManager->GetCoinNum() * kScorePerCoin;

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
	if (m_gameCount > kGameTimeLimit * Game::kFPS)
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
	if (m_finishCount > kFinishInterval)
	{
		SoundManager::GetInstance().StopSound(L"InGameBGM", true);
		m_sceneManager.ChangeScene(std::make_shared<SceneResult>(m_input, m_sceneManager, m_score));
		return;
	}

#ifdef _DEBUG
	// 1キーでシーンを終了
	if (CheckHitKey(KEY_INPUT_1))
	{
		SoundManager::GetInstance().StopSound(L"InGameBGM", true);
		m_sceneManager.ChangeScene(std::make_shared<SceneResult>(m_input, m_sceneManager,m_score));
		return;
	}
	// 2キーで制限時間を1秒にする
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

	// 床用のライトの向きを設定(カメラを上下に動かすと床の反射光がちらつくため、上下の向きを固定)
	auto temp = Matrix4x4::GetRotYMatrix(m_pCamera->GetAngleY()) * kFloorLightDirection;
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

	// ポーズ中でなければUIを描画
	if (!m_isPause)
	{
		DrawUI();
	}

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
	// ゲーム開始前のフレームをカウント
	m_startCount++;

	// カウントダウンの音を鳴らすかどうか
	bool isPlayCountDownSound = false;
	// 適切なタイミングでカウントダウンの音を鳴らす
	if (m_startCount == Game::kFPS * 1) isPlayCountDownSound = true;
	if (m_startCount == Game::kFPS * 2) isPlayCountDownSound = true;
	if (m_startCount == Game::kFPS * 3) isPlayCountDownSound = true;
	if (isPlayCountDownSound)
	{
		SoundManager::GetInstance().PlaySoundGame(L"CountDown");
	}
	// 適切なタイミングでスタートの音を鳴らす
	if (m_startCount == Game::kFPS * 4)
	{
		SoundManager::GetInstance().PlaySoundGame(L"Start");
	}
}

void SceneMain::DrawUI()
{
	// マグネットの描画
	m_pMagnet->Draw();

	// 操作説明の画像の描画
	int w, h;
	GetGraphSize(m_howToPlayGraphHandle, &w ,&h);
	int x = 20;
	int y = Game::kScreenHeight - 20 - h;
	DrawGraph(x, y, m_howToPlayGraphHandle,true);

	// ゲーム開始前のカウントダウンの描画
	DrawStart();

	// ゲーム終了後のテキストの描画
	if (m_finishCount > 0)
	{
		DrawFinish();
	}

	// 残り時間の描画
	unsigned int color = 0xffffff;	// 最初は白で描画
	float sec = kGameTimeLimit - static_cast<float>(m_gameCount) / static_cast<float>(kGameTimeLimit);
	if (sec <= kTimeLimitToRed) color = 0xff0000;	// 残り時間が10秒を切ったら赤で描画
	if (sec < 0) sec = 0.0f;
	std::wstring text = std::format(L"残り時間:{:.1f}", sec);
	auto strWidth = GetDrawFormatStringWidthToHandle(m_uiFontHandle, L"残り時間:60.0");
	x = Game::kScreenWidth / 2 - strWidth / 2;
	DrawFormatStringToHandle(x, kTimeY, color, m_uiFontHandle, text.c_str());

	// スコアの描画
	// 表示用スコアの更新
	// 表示用スコアと実際のスコアの差が定数より大きい場合は、表示用スコアを定数だけ増加させる
	if (m_score - m_dispScore > kScoreDiffToAccelerate)
	{
		m_dispScore += kAcceleratedScoreIncreaseSpeed;
	}
	// 表示用スコアが実際のスコアに追いつくまで、徐々に増加させる
	if (m_dispScore < m_score) m_dispScore += kScoreIncreaseSpeed;
	if (m_dispScore > m_score) m_dispScore = m_score;	// 表示用スコアが実際のスコアを超えないようにする
	// テキストを描画
	text = std::format(L"スコア:{:d}", m_dispScore);
	DrawFormatStringToHandle(x, kUIFontSize + kTimeY, 0xffffff, m_uiFontHandle, text.c_str());
}

void SceneMain::DrawStart()
{
	// フレームから秒数を計算
	int second = m_startCount / Game::kFPS;
	// 5秒以上経っていたら描画しない
	if (second > 4)
	{
		return;
	}
	// 60フレームに満たないうちは描画しない
	if (m_startCount < Game::kFPS)
	{
		return;
	}
	// 開始のカウントダウン秒数テキストを生成
	std::wstring text = L"temp";
	text = std::to_wstring(4 - second);
	// 3秒経ったらSTART!に
	if (second > 3)
	{
		text = L"START!";
		m_isStarted = true;
		m_pPlayer->SetCanControll(true);
	}
	// 透明度を計算 60フレームかけて徐々に消えるのを繰り返す
	float alpha = (Game::kFPS - (m_startCount % Game::kFPS)) / static_cast<float>(Game::kFPS);	// 1.0 ~ 0.0
	alpha *= 2.0f;	// 2.0をかけて2.0~ 0.0の値にする
	alpha *= 255;	// 255をかけて0~510の値にする
	if (alpha > 255) alpha = 255;	// 255を超えないようにする

	// 拡大率を計算 60フレームかけて徐々に大きくなるのを繰り返す
	float scale = (m_startCount % Game::kFPS) / static_cast<float>(Game::kFPS);	// 1.0 ~ 0.0
	scale *= 0.5f;	// 1.3をかけて0~1.3の値にする
	scale += 1.0f;	// 1.0を足して1.0~1.3の値にする

#ifdef _DEBUG
	DrawFormatString(0, 200, 0xffffff, L"float alpha = %.2f", alpha);
#endif

	
	// テキストを描画
	int width, height, lineCount;	// 拡大率も考慮してテキストの描画サイズを取得	// 幅、高さ、行数
	GetDrawExtendFormatStringSizeToHandle(&width, &height, &lineCount, scale, scale, m_titleFontHandle, text.c_str());
	int x = Game::kScreenWidth / 2 - width / 2;	// テキストの描画サイズを元にテキストの描画位置を計算
	int y = Game::kScreenHeight / 2 - height / 2;
	// 事前に計算したアルファ値で半透明にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	// テキストを拡大して描画
	DrawExtendFormatStringToHandle(x, y, scale, scale, 0xffffff, m_titleFontHandle, text.c_str());
	// ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneMain::DrawFinish()
{
#ifdef _DEBUG
	DrawFormatString(0, 200, 0xffffff, L"FinishCount:%d", m_finishCount);
#endif
	// 透明度を計算
	float alpha = (Game::kFPS * 2 - m_finishCount) / static_cast<float>(Game::kFPS) * 2;	// 1.0~0.0
	alpha *= 255 * 2;	// 510~0
	if (alpha > 255) alpha = 255;	// 255を超えないようにする
	// 拡大率を計算
	float scale = m_finishCount / static_cast<float>(Game::kFPS) * 2;	// 0.0~1.0
	scale *= 0.15f;	// 0.0~0.3
	scale += 1.0f;	// 1.0~1.3

	// テキストを描画
	std::wstring text = L"Finish!";
	int width, height, lineCount;
	GetDrawExtendFormatStringSizeToHandle(&width, &height, &lineCount, scale, scale, m_titleFontHandle, text.c_str());
	int x = Game::kScreenWidth / 2 - width / 2;
	int y = Game::kScreenHeight / 2 - height / 2;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawExtendFormatStringToHandle(x, y, scale, scale, 0xffffff, m_titleFontHandle, text.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
