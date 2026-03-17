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

	auto check = GetJoypadXInputState(DX_INPUT_PAD1, &m_nowPadInput);
	assert(check != -1 && "“ü—Í‚ª³‚µ‚­æ“¾‚Å‚«‚Ü‚¹‚ñ‚Å‚µ‚½");
}

bool Input::IsPressed(int button)
{
	return false;
}

bool Input::IsTriggerd(int button)
{
	return false;
}

StickInput Input::GetStickInput(LR lr)
{
	return StickInput();
}

unsigned char Input::GetTriggerInput(LR lr)
{
	return 0;
}
