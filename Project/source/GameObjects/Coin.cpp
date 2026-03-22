#include "Coin.h"
#include "../Geometry.h"
#include <cmath>

namespace
{
	constexpr float kRadius = 100.0f;

	constexpr float kRotationSpeed = 0.05f;

	constexpr float kSpawnVelocity = 10.0f;
	constexpr float kSpawnVelocityY = 20.0f;
}

Coin::Coin():
	GameObject(kRadius)
{
}

Coin::~Coin()
{
}

void Coin::Init()
{
}

void Coin::End()
{
}

void Coin::Update()
{
	// コインを回転させる
	m_angle += kRotationSpeed;
	auto rotMtx = Matrix4x4::GetRotYMatrix(m_angle);

	// 位置に速度を加算して移動
	m_pos += m_vel;
	// 抵抗をかける
	Resistance();
	Gravity();
	// 画面外に出ないようにする
	LimitPos();
	// y0の位置に置くと半分埋まるため、y座標を半径分上げる
	auto pos = m_pos;
	pos.y += kRadius;
	auto transMtx = Matrix4x4::GetTranslateMatrix(pos);
	// コインの当たり判定の位置も更新
	m_sphere.SetPos(pos);

	// 行列を合成してモデルに適用
	auto mtx = transMtx * rotMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
}

void Coin::Draw()
{
	MV1DrawModel(m_modelHandle);
#ifdef _DEBUG
	m_sphere.Draw();
#endif
}

void Coin::OnCollision(const GameObject& other)
{
	// TODO: プレイヤーと当たったときの処理
}

void Coin::Spawn(const Vector3& pos)
{
	// コインの位置を設定
	m_pos = pos;
	m_sphere.SetPos(pos);
	// ランダムな方向に飛ばす
	// ランダムな方向を生成
	float randRad = GetRand(359) * DX_PI_F / 180.0f;
	// 生成した方向に速度を設定
	m_vel.x = sinf(randRad);
	m_vel.z = cosf(randRad);
	m_vel.Normalize();
	m_vel *= kSpawnVelocity;
	m_vel.y = kSpawnVelocityY;
}
