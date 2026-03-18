#include "Camera.h"
#include <cmath>

#include "../GameObjects/Player.h"

namespace
{
	// カメラの初期位置と初期注視点
	const Vector3 kFirstPos = { 0.0f,300.0f,-700.0f };
	const Vector3 kFirstTarget = { 0.0f,0.0f,0.0f };
	// 視野角
	constexpr float kFov = DX_PI_F / 3.0f;	// 60度
	// NearFar
	constexpr float kNear = 200.0f;
	constexpr float kFar = 1500.0f;

	// オフセット
	const Vector3 kTargetOffset = { 0.0f,100.0f,0.0f };
	const Vector3 kPosOffset = { 0.0f,300.0f,0.0f };

	// 注視点との距離
	constexpr float kTargetDis = 800.0f;
}

Camera::Camera(Input& input):
	m_input(input)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	// カメラの初期設定
	m_pos = kFirstPos;
	m_target = kFirstTarget;
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLib(), m_target.ToDxLib());
	SetupCamera_Perspective(kFov);
	SetCameraNearFar(kNear, kFar);

	// 角度を補正
	if (m_angleY < 0.0f) m_angleY += DX_TWO_PI_F;
	if (m_angleY > DX_TWO_PI_F) m_angleY -= DX_TWO_PI_F;
}

void Camera::Update()
{
	// スティック入力に応じて角度を更新
	auto rightStick = m_input.GetStickInput(LR::Right);
	m_angleY -= rightStick.x * 0.05f;
	if (m_angleY < 0.0f) m_angleY += DX_TWO_PI_F;
	if (m_angleY > DX_TWO_PI_F) m_angleY -= DX_TWO_PI_F;

	// プレイヤーの位置をもとにカメラの位置と注視点を設定
	
	// 注視点を設定
	m_target = m_playerPos + kTargetOffset;
	// 位置を設定
	Vector3 temp;
	temp.x = cosf(m_angleY - DX_PI_F / 2);
	temp.z = sinf(m_angleY - DX_PI_F / 2);
	temp *= kTargetDis;
	m_pos = m_playerPos + temp + kPosOffset;

	// 位置と注視点を反映
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLib(), m_target.ToDxLib());
}