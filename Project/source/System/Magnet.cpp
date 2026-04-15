#include "Magnet.h"
#include "Input.h"
#include "Dxlib.h"
#include "../Managers/CoinManager.h"
#include "../GameObjects/Player.h"
#include "../Game.h"

namespace
{
	// 引き寄せのクールダウンタイム
	constexpr int kCooldownTime = Game::kFPS * 10;
}

Magnet::Magnet(Input& input, Player& player, CoinManager& coinManager):
	m_input(input),
	m_player(player),
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
	// クールダウンタイムを減らす
	if (m_cooldown > 0) m_cooldown--;

	// Yボタンが押された
	bool isInput = m_input.IsPressed(XINPUT_BUTTON_Y);
	// クールダウン中でない
	bool isNotCooldown = m_cooldown <= 0;
	// プレイヤーが操作可能な状態
	bool isPlayerCanControll = m_player.IsCanControll();
	// 条件を満たしているなら引き寄せを発動させる
	if (isInput && isNotCooldown && isPlayerCanControll)
	{
		m_coinManager.ActivateAtract();
		m_cooldown = kCooldownTime; // クールダウンタイムを設定
	}
}

void Magnet::Draw()
{
	DrawBox(Game::kScreenWidth - 50 - 150,
		Game::kScreenHeight - 50 - 150,
		Game::kScreenWidth - 50,
		Game::kScreenHeight - 50,
		0x0000ff, true);

	float cooldownRate = static_cast<float>(m_cooldown) / static_cast<float>(kCooldownTime);
	int temp = 150 * cooldownRate;

#ifdef _DEBUG
	DrawFormatString(Game::kScreenWidth - 200, Game::kScreenHeight - 50 - 200, 0xffffff, L"cooldownRate:%.2f", cooldownRate);
	DrawFormatString(Game::kScreenWidth - 200, Game::kScreenHeight - 50 - 200 - 16, 0xffffff, L"temp:%d", temp);
#endif

	DrawBox(Game::kScreenWidth - 50 - 150,
		Game::kScreenHeight - 50 - temp,
		Game::kScreenWidth - 50,
		Game::kScreenHeight - 50,
		0x000088, true);

#ifdef _DEBUG
	// クールダウンタイムを秒数で表示
	DrawFormatString(Game::kScreenWidth - 80, Game::kScreenHeight - 50, 0xffffff, L"M:%.1f", m_cooldown / static_cast<float>(Game::kFPS));
#endif
}
