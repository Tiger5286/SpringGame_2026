#include "Animation.h"

namespace
{
	constexpr float kAnimSpeed = 0.5f;
}

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::Init(int modelHandle)
{
	m_modelHandle = modelHandle;
}

void Animation::End()
{
}

void Animation::Update()
{
}

void Animation::ChangeAnim(int animHandle)
{
}
