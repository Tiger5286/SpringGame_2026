#include "Camera.h"

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
}

Camera::Camera(Player& player):
	m_player(player)
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
}

void Camera::End()
{
}

void Camera::Update()
{
}