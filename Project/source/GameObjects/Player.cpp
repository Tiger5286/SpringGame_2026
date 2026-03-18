#include "Player.h"
#include "../Geometry.h"
#include "../Game.h"
#include <cmath>

namespace
{
	constexpr float kMoveSpeed = 8.0f;
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
	Move();

	if (m_pos.x > Game::kFieldSize) m_pos.x = Game::kFieldSize;
	if (m_pos.x < -Game::kFieldSize) m_pos.x = -Game::kFieldSize;
	if (m_pos.z > Game::kFieldSize) m_pos.z = Game::kFieldSize;
	if (m_pos.z < -Game::kFieldSize) m_pos.z = -Game::kFieldSize;
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}

void Player::Move()
{
	// 左スティックの入力を取得
	const auto leftStick = m_input.GetStickInput(LR::Left);

	// スティックを倒していないときは角度を反映しない
	if (leftStick.SquaredLength() > 0.0f)
	{
		// スティックの入力方向の角度を取得
		m_angle = atan2(leftStick.y, leftStick.x);
		m_angle += DX_PI_F / 2;
		m_angle += m_cameraAngleY;
	}
	// 取得した角度でY軸回転行列を取得
	auto rotYMtx = Matrix4x4::GetRotYMatrix(-m_angle);

	// 移動ベクトルをカメラの角度分回転させる
	auto moveVec = Vector3(leftStick.x, 0.0f, leftStick.y) * kMoveSpeed;
	moveVec = Matrix4x4::GetRotYMatrix(-m_cameraAngleY) * moveVec;
	m_pos += moveVec;

	// プレイヤーの位置に応じた行列を生成
	Matrix4x4 transMtx = Matrix4x4::GetTranslateMatrix(m_pos);

	// 行列を合成
	auto mtx = transMtx * rotYMtx;
	// プレイヤーのモデルに行列を適用
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
}
