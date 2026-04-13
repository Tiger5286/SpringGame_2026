#include "Application.h"
#include "Dxlib.h"
#include "EffekseerForDXLib.h"
#include "Game.h"

#include <memory>
#include <string>

#include "Scenes/SceneManager.h"
#include "Managers/ModelManager.h"
#include "Managers/SoundManager.h"

namespace
{
	// 使用するモデルのファイル名と登録名
	const std::pair<std::wstring, std::wstring> kModelNames[] = {
		{ L"data/Models/Player.MV1", L"Player" },
		{ L"data/Models/Enemy.MV1",  L"Enemy"  },
		{ L"data/Models/Chest.MV1",  L"Chest"  },
		{ L"data/Models/Coin.MV1",   L"Coin"   },
		{ L"data/Models/Floor.MV1",  L"Floor"  }
	};
}

Application& Application::GetInstance()
{
    static Application instance;
    return instance;
}

bool Application::Init()
{
	ChangeWindowMode(true); // ウインドウモードで起動
	SetMainWindowText(L"コインラッシュ！"); // ウインドウのタイトルを設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum); // 画面サイズと色数を設定

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return false;
	}

	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		return false;
	}
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	AddFontResourceEx(L"data/Kaisotai-Next-UP-B.ttf", FR_PRIVATE, NULL); // フォントの追加

	return true;
}

void Application::Run()
{
	// 描画対象をバックバッファに変更
	SetDrawScreen(DX_SCREEN_BACK);

	// カリングの設定
	SetUseBackCulling(true);

	// Zバッファの設定
	SetUseZBuffer3D(true);	// Zバッファを使います
	SetWriteZBuffer3D(true);	// 描画する物体はZバッファにも距離を書き込む

	// ライティングの設定
	SetUseLighting(true);

	// モデルマネージャーの生成
	auto& modelManager = ModelManager::GetInstance();
	// モデルのロード
	for (auto& names : kModelNames)
	{
		modelManager.LoadModel(names.first, names.second);
	}
	// サウンドマネージャーの生成と初期化
	auto& soundManager = SoundManager::GetInstance();
	soundManager.Init();

	// シーンマネージャーの生成と初期化
	auto pSceneManager = std::make_shared<SceneManager>();
	pSceneManager->Init();

	while (ProcessMessage() != -1 && !m_isRequestExit)
	{
		auto start = GetNowHiPerformanceCount(); // フレーム開始時間を取得
		ClearDrawScreen(); // 画面をクリア

		// 更新処理
		pSceneManager->Update();
		soundManager.Update();

		// 描画処理
		pSceneManager->Draw();

		// escキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			RequestExit();
		}

		ScreenFlip(); // 描画した内容を画面に反映する
		while (GetNowHiPerformanceCount() - start < 16667) {} // 約16.667ミリ秒(1/60秒)待つことで60FPSに固定
	}

	// 終了処理
	pSceneManager->End();
	soundManager.End();
}

void Application::Terminate()
{
	RemoveFontResourceEx(L"data/Kaisotai-Next-UP-B.ttf", FR_PRIVATE, NULL); // フォントの削除
	// Effekseerを終了する。
	Effkseer_End();
	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return;				// ソフトの終了 
}