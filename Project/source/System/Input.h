#pragma once
#include "DxLib.h"

enum class LR
{
	Left,
	Right
};

struct StickInput
{
	short horizontal = 0;
	short vertical = 0;
};

class Input
{
public:
	Input();
	virtual ~Input();

	void Update();

	/// <summary>
	/// ボタンが押されているかどうか取得する(引数にはXINPUT_BUTTON定義を使用する)
	/// </summary>
	/// <param name="button">取得したいボタン</param>
	/// <returns>押されているかどうか</returns>
	bool IsPressed(int button);

	/// <summary>
	/// ボタンが押された瞬間を取得する(引数にはXINPUT_BUTTON定義を使用する)
	/// </summary>
	/// <param name="button">取得したいボタン</param>
	/// <returns>押されたかどうか</returns>
	bool IsTriggerd(int button);

	/// <summary>
	/// スティック入力を取得する
	/// </summary>
	/// <param name="lr">左右どちらの入力を取得するか</param>
	/// <returns>スティック入力値</returns>
	StickInput GetStickInput(LR lr);

	/// <summary>
	/// トリガー入力を取得する
	/// </summary>
	/// <param name="lr">左右どちらの入力を取得するか</param>
	/// <returns>トリガー入力値</returns>
	unsigned char GetTriggerInput(LR lr);

private:
	XINPUT_STATE m_nowPadInput;
	XINPUT_STATE m_prevPadInput;
};