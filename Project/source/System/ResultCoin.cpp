#include "ResultCoin.h"
#include "DxLib.h"
#include "../Utility/Matrix4x4.h"

namespace
{
	constexpr float kRotationSpeed = 0.05f;

	const Vector3 kSpawnPos = Vector3(0, 1000, 700);

	constexpr int kSpawnRandomRange = 3000;
}

void ResultCoin::Init(int modelHandle)
{
	m_modelHandle = modelHandle;

	auto rotYMtx = Matrix4x4::GetRotYMatrix(m_angleY);
	auto transMtx = Matrix4x4::GetTranslateMatrix(m_pos);
	auto mtx = transMtx * rotYMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
}

void ResultCoin::End()
{
	MV1DeleteModel(m_modelHandle);
}

void ResultCoin::Update()
{
	m_angleY += kRotationSpeed;
	m_pos.y -= 10.0f;

	auto rotYMtx = Matrix4x4::GetRotYMatrix(m_angleY);
	auto transMtx = Matrix4x4::GetTranslateMatrix(m_pos);
	auto mtx = transMtx * rotYMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());
}

void ResultCoin::Draw()
{
	MV1DrawModel(m_modelHandle);
}

void ResultCoin::Spawn()
{
	m_pos = kSpawnPos;
	float x = GetRand(kSpawnRandomRange) - kSpawnRandomRange / 2;
	m_pos.x = x;
}
