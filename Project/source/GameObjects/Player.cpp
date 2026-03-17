#include "Player.h"
#include "../Geometry.h"

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
	auto leftStick = m_input.GetStickInput(LR::Left);

	// 入力に応じてプレイヤーの位置を更新
	m_pos += Vector3(leftStick.x, 0.0f, leftStick.y) * kMoveSpeed;

	// プレイヤーの位置に応じた行列を生成
	Matrix4x4 translateMtx = Matrix4x4::GetTranslateMatrix(m_pos);

	// プレイヤーのモデルに行列を適用
	MV1SetMatrix(m_modelHandle, translateMtx.ToDxLib());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}