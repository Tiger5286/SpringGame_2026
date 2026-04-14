#include "Fade.h"
#include "../Game.h"
#include "DxLib.h"

namespace
{
	constexpr int kFadeFrame = 40; // フェードにかかるフレーム数
}

void Fade::Update()
{
	m_prevState = m_state; // 前の状態を保存

	// フェードイン中はフェードフレームを減らす
	if (m_state == FadeState::FadeIn)
	{	// フェードフレームが0になるまで減らす
		if (m_fadeFrame > 0)
		{	
			m_fadeFrame--;
		}
		else
		{	// フェードフレームが0になったらフェードイン完了
			m_state = FadeState::NoFade;
		}
	}
	// フェードアウト中はフェードフレームを増やす
	if (m_state == FadeState::FadeOut)
	{
		if (m_fadeFrame < kFadeFrame)
		{	// フェードフレームが最大値になるまで増やす
			m_fadeFrame++;
		}
		else
		{	// フェードフレームが最大値に達したらフェードアウト完了
			m_state = FadeState::NoFade;
		}
	}
}

void Fade::Draw()
{
	// フェード進行度を0.0～1.0の範囲で計算
	float fadeProgress = static_cast<float>(m_fadeFrame) / kFadeFrame;
	float alpha = fadeProgress * 255.0f; // 0.0～255.0の範囲に変換
	// 黒フィルターを描画
	DrawFade(alpha);

#ifdef _DEBUG
	DrawFormatString(50, 150, 0xffffff, L"FadeState: %d", static_cast<int>(m_state));
	DrawFormatString(50, 166, 0xffffff, L"FadeFrame: %d", m_fadeFrame);
#endif
}

void Fade::StartFadeIn()
{
	if (m_state != FadeState::NoFade)
	{
		return; // フェード中はフェードインを開始しない
	}
	m_state = FadeState::FadeIn;
	m_fadeFrame = kFadeFrame;
}

void Fade::StartFadeOut()
{
	if (m_state != FadeState::NoFade)
	{
		return; // フェード中はフェードアウトを開始しない
	}
	m_state = FadeState::FadeOut;
	m_fadeFrame = 0;
}

bool Fade::IsFadeEnd()
{
	// 現在のフレームがフェード無し、かつ前回のフレームがフェード中であれば、フェードが終わった瞬間と判断する
	return m_state == FadeState::NoFade && m_prevState != FadeState::NoFade;
}

void Fade::DrawFade(float alpha)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight,0x000000,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
