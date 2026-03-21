#include "Coin.h"
#include "../Geometry.h"

namespace
{
	constexpr float kRadius = 100.0f;

	constexpr float kRotationSpeed = 0.05f;
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
}
