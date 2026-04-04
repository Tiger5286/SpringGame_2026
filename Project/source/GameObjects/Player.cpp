#include "Player.h"
#include "../Geometry.h"
#include <cmath>
#include <string>
#include "PunchCollider.h"
#include "../Managers/CollisionManager.h"

namespace
{
	// 移動速度
	constexpr float kMoveSpeed = 10.0f;
	// アニメーション名
	const std::wstring kIdleAnimName = L"CharacterArmature|Idle";
	const std::wstring kMoveAnimName = L"CharacterArmature|Run";
	const std::wstring kPunchAnimName = L"CharacterArmature|Punch";
	const std::wstring kHitAnimName = L"CharacterArmature|HitReact";

	// 当たり判定の半径
	constexpr float kSphereRadius = 100.0f;

	// パンチする時間
	constexpr int kPunchFrame = 20;
	// パンチアニメーションの再生速度
	constexpr float kPunchAnimSpeed = 1.0f;
	// プレイヤーからパンチコライダーまでの距離
	constexpr float kPunchDistance = 100.0f;

	// 被弾アニメーションの長さ(フレーム)
	constexpr int kHitAnimFrame = 18 * 2;
	// 無敵時間の長さ(フレーム)
	constexpr int kInvincibleFrame = 120;
	// 無敵時間の点滅の間隔(フレーム)
	constexpr int kInvincibleFrickerInterval = 12;
}

Player::Player(Input& input, std::shared_ptr<CollisionManager> pCollisionManager, std::shared_ptr<EffectManager> pEffectManager) :
	GameObject(kSphereRadius),
	m_input(input),
	m_pCollisionManager(pCollisionManager),
	m_pEffectManager(pEffectManager)
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
	// 被弾中でなければパンチ
	if (!m_isHitEnemy)
	{
		Punch();
	}
	// 被弾
	Hit();

	// 無敵時間の更新
	if (m_isInvincible)
	{
		m_invincibleFrame++;
		if (m_invincibleFrame >= kInvincibleFrame)
		{
			m_invincibleFrame = 0;
			m_isInvincible = false;
		}
	}

	// ステートの更新
	UpdateState();
	// アニメーションの更新
	UpdateAnimation();
}

void Player::Draw()
{
	if (m_isInvincible)
	{
		if (m_invincibleFrame % kInvincibleFrickerInterval * 2 < kInvincibleFrickerInterval)
		{
			MV1DrawModel(m_modelHandle);
		}
	}
	else
	{
		MV1DrawModel(m_modelHandle);
	}
	
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
	// 敵に当たった
	if (other.GetTag() == ObjectTag::Enemy)
	{
		// パンチ中でなければ、かつ無敵でなければ
		if (m_punchFrame == 0 && !m_isInvincible)
		{
			// 当たった処理をする
			m_isHitEnemy = true;
			m_isCanControll = false;
		}
	}
}

void Player::Move()
{
	// 左スティックの入力を取得
	auto leftStick = m_input.GetStickInput(LR::Left);
	// 入力できない状態なら入力を無効にする
	if (!m_isCanControll)
	{
		leftStick = Vector3(0.0f, 0.0f, 0.0f);
	}

	// スティックを倒していないときは角度を反映しない
	if (leftStick.SquaredLength() > 0.0f)
	{
		// スティックの入力方向の角度を取得
		m_angle = atan2(leftStick.y, -leftStick.x);
		m_angle += DX_PI_F / 2;
		m_angle += m_cameraAngleY;
	}
	// 取得した角度でY軸回転行列を取得
	auto rotYMtx = Matrix4x4::GetRotYMatrix(m_angle);

	// 移動ベクトルをカメラの角度分回転させる
	auto moveVec = Vector3(leftStick.x, 0.0f, leftStick.y) * kMoveSpeed;
	moveVec = Matrix4x4::GetRotYMatrix(m_cameraAngleY) * moveVec;
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
	if (m_pCollisionManager == nullptr) return;

	// Bボタンが押されたとき、入力が有効なら
	if (m_input.IsTriggerd(XINPUT_BUTTON_B) && m_isCanControll)
	{
		// パンチ中でなければパンチする
		if (m_punchFrame == 0)
		{
			// パンチフレームをリセット
			m_punchFrame = kPunchFrame;
			// パンチコライダーを生成
			m_pPunchCollider = std::make_shared<PunchCollider>(*m_pEffectManager);
			m_pPunchCollider->Init();
			m_pCollisionManager->Register(m_pPunchCollider);
			// パンチコライダーの位置をプレイヤーの前方に設定
			float punchRadius = m_pPunchCollider->GetSphere().GetRadius();
			Vector3 temp = { 0,0,-kPunchDistance };
			temp = Matrix4x4::GetRotYMatrix(m_angle) * temp;
			temp.y = punchRadius;
			Vector3 punchPos = m_pos + temp;
			m_pPunchCollider->SetPos(punchPos);
		}
	}
	// パンチ中はカウントを減らし、位置をプレイヤーの前方に設定し続ける
	if (m_punchFrame > 0)
	{
		m_punchFrame--;
		float punchRadius = m_pPunchCollider->GetSphere().GetRadius();
		Vector3 temp = { 0,0,-kPunchDistance };
		temp = Matrix4x4::GetRotYMatrix(m_angle) * temp;
		temp.y = punchRadius;
		Vector3 punchPos = m_pos + temp;
		m_pPunchCollider->SetPos(punchPos);
	}
	// パンチ中でないときはパンチコライダーを消す
	if (m_punchFrame == 0)
	{
		m_pCollisionManager->Unregister(m_pPunchCollider);
		m_pPunchCollider = nullptr;
	}
}

void Player::Hit()
{
	if (m_isHitEnemy)
	{
		m_hitFrame++;
		if (m_hitFrame >= kHitAnimFrame)
		{
			m_isHitEnemy = false;
			m_isCanControll = true;
			m_isInvincible = true;
			m_hitFrame = 0;
		}
	}
}

void Player::UpdateState()
{
	if (m_punchFrame > 0) // パンチ中
	{
		m_state = PlayerState::Punch;
	}
	else if (m_isHitEnemy)
	{
		m_state = PlayerState::Hit;
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
	if (TriggeredChangeState(PlayerState::Hit))
	{
		m_animation.ChangeAnim(kHitAnimName);
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
