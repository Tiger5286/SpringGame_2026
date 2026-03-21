#include "Enemy.h"
#include "Dxlib.h"
#include <cmath>
#include <string>

#include "Player.h"

namespace
{
	// 移動速度
	constexpr float kMoveSpeed = 4.0f;
	// プレイヤーとの最短距離
	constexpr float kMinDistanceToPlayer = 50.0f;

	// アニメーション名
	const std::wstring kAnimName = L"MonsterArmature|Flying";
	// アニメーションの再生速度
	constexpr float kAnimSpeed = 0.5f;

	// 当たり判定の半径
	constexpr float kSphereRadius = 80.0f;

	// プレイヤーに吹きとばされたときの横の速度
	constexpr float kHitPunchSpeed = 20.0f;
	// プレイヤーに吹きとばされたときの上への速度
	constexpr float kHitPunchUpSpeed = 30.0f;
	// 吹き飛ばされてから死ぬまでの時間(フレーム)
	constexpr int kHitPunchDuration = 30;
	
}

Enemy::Enemy(Player& player):
	GameObject(kSphereRadius),
	m_player(player)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	// アニメーションをアタッチ
	m_animHandle = MV1AttachAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, kAnimName.c_str()));
	m_tag = ObjectTag::Enemy;
}

void Enemy::End()
{
}

void Enemy::Update()
{
	Move();

	if (m_isHitPunch)
	{
		Gravity();
		m_hitPunchFrame++;
		if (m_hitPunchFrame >= kHitPunchDuration)
		{
			m_isDead = true;
		}
	}

	// アニメーション
	// アニメーションの進行
	m_animTime += kAnimSpeed;
	// アニメーションのループ
	float totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animHandle);
	while (m_animTime >= totalTime)
	{
		m_animTime -= totalTime;
	}
	MV1SetAttachAnimTime(m_modelHandle, m_animHandle, m_animTime);
}

void Enemy::Draw()
{
	MV1DrawModel(m_modelHandle);
#ifdef _DEBUG
	m_sphere.Draw();
#endif
}

void Enemy::OnCollision(const GameObject& other)
{
	// TODO: 当たったときの処理
	if (other.GetTag() == ObjectTag::PunchCollider)
	{
		OnHitPunch();
	}
}

void Enemy::Spawn(const Vector3& spawnPos)
{
	m_pos = spawnPos;
}

void Enemy::OnHitPunch()
{
	// プレイヤーと反対方向に吹きとんで死ぬ
	m_isHitPunch = true;

	auto playerToEnemy = m_pos - m_player.GetPos();
	auto knockBackVec = playerToEnemy.Normalized() * kHitPunchSpeed;
	knockBackVec.y = kHitPunchUpSpeed;
	m_vel = knockBackVec;
}

void Enemy::Move()
{
	// プレイヤーの方向に移動する
	// プレイヤーの位置を取得
	const auto playerPos = m_player.GetPos();
	// 敵からプレイヤーまでのベクトルを生成
	auto enemyToPlayer = playerPos - m_pos;
	// プレイヤーとの距離が遠い、かつパンチを受けていないときだけ移動する
	bool isFarFromPlayer = enemyToPlayer.SquaredLength() > kMinDistanceToPlayer * kMinDistanceToPlayer;
	if (isFarFromPlayer && !m_isHitPunch)
	{
		// ベクトルを移動用に加工
		enemyToPlayer.Normalize();
		enemyToPlayer *= kMoveSpeed;
		// 位置に足す
		m_pos += enemyToPlayer;
	}
	// 当たり判定の位置を設定
	m_sphere.SetPos({ m_pos.x,m_pos.y + kSphereRadius,m_pos.z });
	// 平行移動行列を生成
	auto transMtx = Matrix4x4::GetTranslateMatrix(m_pos);

	// プレイヤーの方を向く
	// プレイヤーへのベクトルの角度を出す
	auto angle = atan2(enemyToPlayer.z, enemyToPlayer.x);
	angle += DX_PI_F / 2;
	// 回転行列を生成
	auto rotMtx = Matrix4x4::GetRotYMatrix(-angle);

	// 行列を合成
	auto mtx = transMtx * rotMtx;
	// 行列をモデルに適用
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
}
