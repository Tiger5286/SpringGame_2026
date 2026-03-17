#include "Input.h"
#include <cassert>

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
		stick.horizontal = m_nowPadInput.ThumbLX;
		stick.vertical = m_nowPadInput.ThumbLY;
	}
	else if (lr == LR::Right)
	{
		stick.horizontal = m_nowPadInput.ThumbRX;
		stick.vertical = m_nowPadInput.ThumbRY;
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
