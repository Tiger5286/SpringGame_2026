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

	// UIの画面端からの距離(余白)
	constexpr int kUIMargin = 50;

	// UIの背景のスケール
	constexpr float kUIBackScale = 0.3f;
	// UIの背景の回転速度
	constexpr float kUIBackRotateSpeed = 0.05f;
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
	// アイコンの画像をロード
	m_uiGraphHandle = LoadGraph(L"data/Graphs/magnet_ui.png");
	m_uiBackGraphHandle = LoadGraph(L"data/Graphs/titleBack.png");
}

void Magnet::End()
{
	// アイコンの画像を削除
	DeleteGraph(m_uiGraphHandle);
	DeleteGraph(m_uiBackGraphHandle);
}

void Magnet::Update()
{
	// クールダウンタイムを減らす
	if (m_cooldown > 0) m_cooldown--;

	// UIの背景を回転させる
	m_uiBackAngle += kUIBackRotateSpeed;

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
	int w, h;
	GetGraphSize(m_uiGraphHandle, &w, &h);

	// アイコンを暗く描画
	SetDrawBright(128, 128, 128);
	DrawGraph(Game::kScreenWidth - w - kUIMargin, Game::kScreenHeight - h - kUIMargin, m_uiGraphHandle, true);
	SetDrawBright(255, 255, 255);

	// 現在のクールタイムの割合を計算
	float cooldownRate = static_cast<float>(m_cooldown) / static_cast<float>(kCooldownTime);

	// クールタイムが終わっていたら
	if (cooldownRate <= 0.0f)
	{
		// UIの背景を描画
		DrawRotaGraph(Game::kScreenWidth - w / 2 - kUIMargin,
			Game::kScreenHeight - h / 2 - kUIMargin,
			kUIBackScale, m_uiBackAngle, m_uiBackGraphHandle, true);
	}

	// クールタイムに合わせて扇を大きくするように描画
	DrawCircleGauge(Game::kScreenWidth - w / 2 - kUIMargin,
		Game::kScreenHeight - h / 2 - kUIMargin,
		(1.0f - cooldownRate) * 100,
		m_uiGraphHandle);


#ifdef _DEBUG
	DrawFormatString(Game::kScreenWidth - 200, Game::kScreenHeight - 50 - 200, 0xffffff, L"cooldownRate:%.2f", cooldownRate);
#endif

#ifdef _DEBUG
	// クールダウンタイムを秒数で表示
	DrawFormatString(Game::kScreenWidth - 80, Game::kScreenHeight - 50, 0xffffff, L"sec:%.1f", m_cooldown / static_cast<float>(Game::kFPS));
#endif
}