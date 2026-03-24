#include "Player.h"
#include "../Geometry.h"
#include "../Game.h"
#include <cmath>
#include <string>
#include "PunchCollider.h"
#include "../Managers/CollisionManager.h"

namespace
{
	// 移動速度
	constexpr float kMoveSpeed = 8.0f;
	// アニメーション名
	const std::wstring kIdleAnimName = L"CharacterArmature|Idle";
	const std::wstring kMoveAnimName = L"CharacterArmature|Run";
	const std::wstring kPunchAnimName = L"CharacterArmature|Punch";

	// 当たり判定の半径
	constexpr float kSphereRadius = 80.0f;

	// パンチする時間
	constexpr int kPunchFrame = 20;
	// パンチアニメーションの再生速度
	constexpr float kPunchAnimSpeed = 1.0f;
	// プレイヤーからパンチコライダーまでの距離
	constexpr float kPunchDistance = 100.0f;
}

Player::Player(Input& input, CollisionManager& collisionManager) :
	GameObject(kSphereRadius),
	m_input(input),
	m_collisionManager(collisionManager)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_animation.Init(m_modelHandle, kIdleAnimName);
	m_tag = ObjectTag::Player;
}

void Player::End()
{
}

void Player::Update()
{
	// 前のフレームの状態を保存
	m_prevState = m_state;
	m_prevPos = m_pos;

	// 移動
	Move();
	// パンチ
	Punch();
	// ステートの更新
	UpdateState();
	// アニメーションの更新
	UpdateAnimation();
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
#ifdef _DEBUG
	// 当たり判定の描画
	m_sphere.Draw();
	// パンチの当たり判定の描画
	if (m_pPunchCollider != nullptr)
	{
		m_pPunchCollider->Draw();
	}
#endif
}

void Player::OnCollision(const GameObject& other)
{
	if (other.GetTag() == ObjectTag::Enemy)
	{
		DrawCircle(100, 100, 50, 0xff0000, true);
	}
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

	LimitPos();

	m_sphere.SetPos({ m_pos.x,m_pos.y + kSphereRadius,m_pos.z });

	// プレイヤーの位置に応じた行列を生成
	Matrix4x4 transMtx = Matrix4x4::GetTranslateMatrix(m_pos);

	// 行列を合成
	auto mtx = transMtx * rotYMtx;
	// プレイヤーのモデルに行列を適用
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
}

void Player::Punch()
{
	// Bボタンが押されたとき、パンチが終わっていたらパンチする
	if (m_input.IsTriggerd(XINPUT_BUTTON_B))
	{
		if (m_punchFrame == 0)
		{
			// パンチフレームをリセット
			m_punchFrame = kPunchFrame;
			// パンチコライダーを生成
			m_pPunchCollider = std::make_shared<PunchCollider>();
			m_pPunchCollider->Init();
			m_collisionManager.Register(m_pPunchCollider);
			// パンチコライダーの位置をプレイヤーの前方に設定
			float punchRadius = m_pPunchCollider->GetSphere().GetRadius();
			Vector3 punchPos = Vector3(sinf(-m_angle + DX_PI_F) * kPunchDistance, punchRadius, cosf(-m_angle + DX_PI_F) * kPunchDistance) + m_pos;
			m_pPunchCollider->SetPos(punchPos);
		}
	}
	// パンチ中はカウントを減らし、位置をプレイヤーの前方に設定し続ける
	if (m_punchFrame > 0)
	{
		m_punchFrame--;
		float punchRadius = m_pPunchCollider->GetSphere().GetRadius();
		Vector3 punchPos = Vector3(sinf(-m_angle + DX_PI_F) * kPunchDistance, punchRadius, cosf(-m_angle + DX_PI_F) * kPunchDistance) + m_pos;
		m_pPunchCollider->SetPos(punchPos);
	}
	// パンチ中でないときはパンチコライダーを消す
	if (m_punchFrame == 0)
	{
		m_collisionManager.Unregister(m_pPunchCollider);
		m_pPunchCollider = nullptr;
	}
}

void Player::UpdateState()
{
	if (m_punchFrame > 0) // パンチ中
	{
		m_state = PlayerState::Punch;
	}
	else if (m_prevPos != m_pos) // 前の位置と現在の位置が違っている=移動している
	{
		m_state = PlayerState::Move;

	}
	else
	{
		m_state = PlayerState::Idle;
	}
}

void Player::UpdateAnimation()
{
	// ステートが変わった瞬間を取得
	if (TriggeredChangeState(PlayerState::Punch))
	{
		m_animation.ChangeAnim(kPunchAnimName, kPunchAnimSpeed);
	}
	if (TriggeredChangeState(PlayerState::Move))
	{
		m_animation.ChangeAnim(kMoveAnimName);
	}
	if (TriggeredChangeState(PlayerState::Idle))
	{
		m_animation.ChangeAnim(kIdleAnimName);
	}
	// アニメーションの更新
	m_animation.Update();
}

bool Player::TriggeredChangeState(PlayerState state) const
{
	if (m_state == state && m_prevState != state) return true;
	return false;
}
