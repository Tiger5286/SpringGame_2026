#include "DxLib.h"
#include "Game.h"
#include <memory>

#include "Scenes/SceneMain.h"
#include "System/Input.h"

// プログラムは WinMain から始まる
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true); // ウインドウモードで起動
	SetMainWindowText(L"SpringGame2026"); // ウインドウのタイトルを設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum); // 画面サイズと色数を設定
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	// 描画対象をバックバッファに変更
	SetDrawScreen(DX_SCREEN_BACK);

	// カリングの設定
	SetUseBackCulling(true);

	// Zバッファの設定
	SetUseZBuffer3D(true);	// Zバッファを使います
	SetWriteZBuffer3D(true);	// 描画する物体はZバッファにも距離を書き込む

	Input input;

	auto pScene = std::make_shared<SceneMain>(input);
	pScene->Init();

	while (ProcessMessage() != -1)
	{
		LONGLONG start = GetNowHiPerformanceCount(); // フレーム開始時間を取得
		ClearDrawScreen(); // 画面をクリア

		input.Update();
		pScene->Update();
		pScene->Draw();

		// escキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
		ScreenFlip(); // 描画した内容を画面に反映する
		while (GetNowHiPerformanceCount() - start < 16667) {} // 約16.667ミリ秒(1/60秒)待つことで60FPSに固定
	}
	pScene->End();

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}