#include "Magnet.h"
#include "Input.h"
#include "Dxlib.h"
#include "../Managers/CoinManager.h"
#include "../GameObjects/Player.h"
#include "../Game.h"
#include "../Managers/SoundManager.h"
#include "../Managers/EffectManager.h"
#include <cassert>

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

	const std::vector<std::wstring> kGraphFileNames = {
		L"data/Graphs/magnet_ui.png",
		L"data/Graphs/titleBack.png",
		L"data/Graphs/button_y.png",
		L"data/Graphs/button_y_outline.png"
	};
}

enum class GraphType
{
	UI,
	UIBack,
	YButton,
	YButtonOutline,

	Num
};

Magnet::Magnet(Input& input, Player& player, CoinManager& coinManager, EffectManager& effectManager):
	m_input(input),
	m_player(player),
	m_coinManager(coinManager),
	m_effectManager(effectManager)
{
	// 使用する画像数とロードする画像数が合ってなかったらエラー
	assert(static_cast<int>(GraphType::Num) == kGraphFileNames.size());
}

Magnet::~Magnet()
{
}

void Magnet::Init()
{
	// 画像をロード
	for (auto& fileName : kGraphFileNames)
	{
		auto handle = LoadGraph(fileName.c_str());
		assert(handle != -1 && "画像が正しくロードされませんでした");
		m_graphHandles.push_back(handle);
	}
}

void Magnet::End()
{
	// 画像を削除
	for (auto& handle : m_graphHandles)
	{
		DeleteGraph(handle);
	}
}

void Magnet::Update()
{
	m_frameCount++;

	auto& soundManager = SoundManager::GetInstance();
	Vector3 effectPos = m_player.GetPos() + Vector3(0, 150, 0);

	// クールダウンタイムを減らす
	if (m_cooldown > 0) m_cooldown--;

	// クールタイムが終わるタイミングで音を鳴らし、エフェクトを出す
	if (m_cooldown == 1)
	{
		soundManager.PlaySoundGame(L"MagnetRecharge");
		m_effectManager.PlayEffect(L"MagnetRecharge", effectPos);
	}

	// UIの背景を回転させる
	m_uiBackAngle += kUIBackRotateSpeed;

	// Yボタンが押された
	bool isInput = m_input.IsTriggerd(XINPUT_BUTTON_Y);
	// クールダウン中でない
	bool isNotCooldown = m_cooldown <= 0;
	// プレイヤーが操作可能な状態
	bool isPlayerCanControll = m_player.IsCanControll();
	// 条件を満たしているなら引き寄せを発動させる
	if (isInput && isPlayerCanControll)
	{
		// 引き寄せ発動
		if (isNotCooldown)
		{
			m_coinManager.ActivateAtract();
			m_cooldown = kCooldownTime; // クールダウンタイムを設定
			// 音を鳴らし、エフェクトを出す
			soundManager.PlaySoundGame(L"MagnetActive");
			m_effectManager.PlayEffect(L"MagnetActive", effectPos);
		}
		else	// クールタイム中なら発動できない音を鳴らす
		{
			soundManager.PlaySoundGame(L"No");
		}
	}
}

void Magnet::Draw()
{
	int w, h;
	GetGraphSize(m_graphHandles[static_cast<int>(GraphType::UI)], &w, &h);

	// アイコンを暗く描画
	SetDrawBright(128, 128, 128);
	DrawGraph(Game::kScreenWidth - w - kUIMargin, Game::kScreenHeight - h - kUIMargin, m_graphHandles[static_cast<int>(GraphType::UI)], true);
	SetDrawBright(255, 255, 255);

	// 現在のクールタイムの割合を計算
	float cooldownRate = static_cast<float>(m_cooldown) / static_cast<float>(kCooldownTime);

	// クールタイムが終わっていたら
	if (cooldownRate <= 0.0f)
	{
		// UIの背景を描画
		DrawRotaGraph(Game::kScreenWidth - w / 2 - kUIMargin,
			Game::kScreenHeight - h / 2 - kUIMargin,
			kUIBackScale, m_uiBackAngle, m_graphHandles[static_cast<int>(GraphType::UIBack)], true);
	}

	// クールタイムに合わせて扇を大きくするように描画
	DrawCircleGauge(Game::kScreenWidth - w / 2 - kUIMargin,
		Game::kScreenHeight - h / 2 - kUIMargin,
		(1.0f - cooldownRate) * 100,
		m_graphHandles[static_cast<int>(GraphType::UI)]);

	// クールタイムが終わっていたら
	if (cooldownRate <= 0.0f)
	{
		// ボタンUIのサイズを取得
		int buttonW, buttonH;
		GetGraphSize(m_graphHandles[static_cast<int>(GraphType::YButton)], &buttonW, &buttonH);

		// YボタンのUIを描画
		int x = Game::kScreenWidth - kUIMargin - w - buttonW / 2 + 20;
		int y = Game::kScreenHeight - kUIMargin - buttonH / 2 - 20;
		if (m_frameCount % 20 < 10)
		{
			DrawGraph(x, y, m_graphHandles[static_cast<int>(GraphType::YButton)], true);
		}
		else
		{
			DrawGraph(x, y, m_graphHandles[static_cast<int>(GraphType::YButtonOutline)], true);
		}
	}


#ifdef _DEBUG
	DrawFormatString(Game::kScreenWidth - 200, Game::kScreenHeight - 50 - 200, 0xffffff, L"cooldownRate:%.2f", cooldownRate);
#endif

#ifdef _DEBUG
	// クールダウンタイムを秒数で表示
	DrawFormatString(Game::kScreenWidth - 80, Game::kScreenHeight - 50, 0xffffff, L"sec:%.1f", m_cooldown / static_cast<float>(Game::kFPS));
#endif
}