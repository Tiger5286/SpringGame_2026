#include "Player.h"
#include "../Geometry.h"
#include <cmath>

namespace
{
	constexpr float kMoveSpeed = 5.0f;
}

Player::Player(Input& input) :
	m_input(input)
{
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::End()
{
}

void Player::Update()
{
	// 左スティックの入力を取得
	const auto leftStick = m_input.GetStickInput(LR::Left);

	// スティックを倒していないときは角度を反映しない
	if (leftStick.SquaredLength() > 0.0f)
	{
		// スティックの入力方向の角度を取得
		// xだけなぜかモデルが反転するのでスティックのx入力を反転
		m_angle = atan2(leftStick.y, -leftStick.x);
		// モデルの向きを正しくするため90度回転
		m_angle += DX_PI_F / 2;
	}
	// 取得した角度でY軸回転行列を取得
	auto rotYMtx = Matrix4x4::GetRotYMatrix(m_angle);

	// 入力に応じてプレイヤーの位置を更新
	m_pos += Vector3(leftStick.x, 0.0f, leftStick.y) * kMoveSpeed;
	// プレイヤーの位置に応じた行列を生成
	Matrix4x4 transMtx = Matrix4x4::GetTranslateMatrix(m_pos);

	// 行列を合成
	auto mtx = transMtx * rotYMtx;
	// プレイヤーのモデルに行列を適用
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}