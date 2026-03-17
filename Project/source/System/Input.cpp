#include "Input.h"
#include <cassert>
#include <cmath>

namespace
{
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
}

bool Input::IsPressed(int button)
{
	return m_nowPadInput.Buttons[button];
}

bool Input::IsTriggerd(int button)
{
	return m_nowPadInput.Buttons[button] && !m_prevPadInput.Buttons[button];
}

StickInput Input::GetStickInput(LR lr)
{
	StickInput stick;
	if (lr == LR::Left)
	{
		stick.x = m_nowPadInput.ThumbLX;
		stick.y = m_nowPadInput.ThumbLY;
	}
	else if (lr == LR::Right)
	{
		stick.x = m_nowPadInput.ThumbRX;
		stick.y = m_nowPadInput.ThumbRY;
	}
	return stick;
}

unsigned char Input::GetTriggerInput(LR lr)
{
	if (lr == LR::Left)
	{
		return m_nowPadInput.LeftTrigger;
	}
	else if (lr == LR::Right)
	{
		return m_nowPadInput.RightTrigger;
	}
	return 0;
}

StickInput Input::ConvertStickInput(StickInput stick)
{
	if (stick.x < kMinStickValue) stick.x = kMinStickValue;
	if (stick.x > kMaxStickValue) stick.x = kMaxStickValue;
	if (stick.y < kMinStickValue) stick.y = kMinStickValue;
	if (stick.y > kMaxStickValue) stick.y = kMaxStickValue;

	float len = sqrtf(stick.x * stick.x + stick.y * stick.y);

	return StickInput();
}
