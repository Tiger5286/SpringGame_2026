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
	// 敵が地面から浮いている距離
	constexpr float kFloatDist = 0.0f;

	// アニメーション名
	const std::wstring kAnimName = L"MonsterArmature|Walk";
	const std::wstring kDeathAnimName = L"MonsterArmature|Death";
	// アニメーションの再生速度
	constexpr float kAnimSpeed = 0.5f;

	// 拡大率
	constexpr float kModelScale = 1.5f;
	// 当たり判定の半径
	constexpr float kSphereRadius = 120.0f;

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
	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kAnimName);
	// タグを設定
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

	// アニメーションの更新
	m_anim.Update();
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
	// パンチと当たったとき
	if (other.GetTag() == ObjectTag::Punch)
	{
		OnHitPunch();
		m_anim.ChangeAnim(kDeathAnimName,0.35f);
	}
	// 敵と当たったとき
	if (other.GetTag() == ObjectTag::Enemy)
	{
		PushBack(other.GetSphere().GetPos(), other.GetSphere().GetRadius());
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
		// 高さを固定
		m_pos.y = kFloatDist;
	}
	LimitPos();
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

	// 拡大行列を生成
	auto scaleMtx = Matrix4x4::GetScaleMatrix(Vector3(kModelScale, kModelScale, kModelScale));

	// 行列を合成
	auto mtx = transMtx * rotMtx * scaleMtx;
	// 行列をモデルに適用
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
}

void Enemy::PushBack(const Vector3& center, float radius)
{
	auto otherToThis = m_pos - center;

	auto distance = otherToThis.Length();

	auto diff = kSphereRadius + radius - distance;

	otherToThis.Normalize();
	otherToThis *= diff;

	m_pos += otherToThis;
}
