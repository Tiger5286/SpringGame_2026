#include "Magnet.h"
#include "Input.h"
#include "Dxlib.h"
#include "../Managers/CoinManager.h"

Magnet::Magnet(Input& input, CoinManager& coinManager):
	m_input(input),
	m_coinManager(coinManager)
{
}

Magnet::~Magnet()
{
}

void Magnet::Init()
{
}

void Magnet::End()
{
}

void Magnet::Update()
{
	// Yボタンが押された瞬間に引き寄せを発動させる
	if (m_input.IsTriggerd(XINPUT_BUTTON_Y))
	{
		m_coinManager.ActivateAtract();
	}
}

void Magnet::Draw()
{
}
