#include "Input.h"
#include <cassert>
#include <cmath>

namespace
{
	// スティック入力のデッドゾーンの範囲
	constexpr int kMinStickValue = 3000;
	constexpr int kMaxStickValue = 25000;
}

Input::Input()
{
}

Input::~Input()
{
}

void Input::Update()
{
	m_prevPadInput = m_nowPadInput;

	GetJoypadXInputState(DX_INPUT_PAD1, &m_nowPadInput);

	DrawFormatString(100, 100, 0xffffff, L"%.2f", GetTriggerInput(LR::Left));
}

bool Input::IsPressed(int button)
{
	return m_nowPadInput.Buttons[button];
}

bool Input::IsTriggerd(int button)
{
	return m_nowPadInput.Buttons[button] && !m_prevPadInput.Buttons[button];
}

Vector2 Input::GetStickInput(LR lr)
{
	Vector2 stick;
	if (lr == LR::Left)
	{
		stick.x = m_nowPadInput.ThumbLX;
		stick.y = m_nowPadInput.ThumbLY;
		stick = ConvertStickInput(stick);
	}
	else if (lr == LR::Right)
	{
		stick.x = m_nowPadInput.ThumbRX;
		stick.y = m_nowPadInput.ThumbRY;
		stick = ConvertStickInput(stick);
	}
	return stick;
}

float Input::GetTriggerInput(LR lr)
{
	if (lr == LR::Left)
	{
		return ConvertTriggerInput(m_nowPadInput.LeftTrigger);
	}
	else if (lr == LR::Right)
	{
		return ConvertTriggerInput(m_nowPadInput.RightTrigger);
	}
	return 0;
}

Vector2 Input::ConvertStickInput(Vector2 stick)
{
	// スティック入力のデッドゾーンを設定
	if (stick.x < kMinStickValue) stick.x = kMinStickValue;
	if (stick.x > kMaxStickValue) stick.x = kMaxStickValue;
	if (stick.y < kMinStickValue) stick.y = kMinStickValue;
	if (stick.y > kMaxStickValue) stick.y = kMaxStickValue;

	// スティック入力を-1.0~1.0の範囲に変換
	stick.x = (stick.x - kMinStickValue) / static_cast<float>(kMaxStickValue - kMinStickValue) * 2.0f - 1.0f;
	stick.y = (stick.y - kMinStickValue) / static_cast<float>(kMaxStickValue - kMinStickValue) * 2.0f - 1.0f;

	return stick;
}

float Input::ConvertTriggerInput(unsigned char trigger)
{
	return trigger / 255.0f;
}
