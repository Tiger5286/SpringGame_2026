#include "Quaternion.h"
#include <cmath>

Quaternion::Quaternion():
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(0.0f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w):
	x(x),
	y(y),
	z(z),
	w(w)
{
}

Quaternion::~Quaternion()
{
}

void Quaternion::DrawAxis()
{
	Vector3 vec = Vector3(x, y, z);
	vec.Normalize();
	vec *= 20;
	DrawLine3D(VGet(0, 0, 0), vec.ToDxLib(), 0x00ffff);
	DrawSphere3D(vec.ToDxLib(), 2, 8, 0xff0000, 0xff0000, true);
}

Quaternion Quaternion::CreateQuaternion(Vector3 axis, float eulerAngle)
{
	// 軸ベクトルを正規化
	axis.Normalize();
	// オイラー角をラジアンに変換
	float rad = eulerAngle * (DX_PI_F / 180.0f);
	// クォータニオンを計算
	return Quaternion(axis.x * sinf(rad / 2.0f),
		axis.y * sinf(rad / 2.0f),
		axis.z * sinf(rad / 2.0f),
		cosf(rad / 2.0f));
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	return Quaternion(
		x * other.w + y * other.z - z * other.y + w * other.x,
		-x * other.z + y * other.w + z * other.x + w * other.y,
		x * other.y - y * other.x + z * other.w + w * other.z,
		-x * other.x - y * other.y - z * other.z + w * other.w
	);
}

void Quaternion::operator*=(const Quaternion& other)
{
	*this = *this * other;
}

Vector3 Quaternion::operator*(const Vector3& vec) const
{
	// ベクトルをクォータニオンとしてでっちあげる
	Quaternion Vq(vec.x, vec.y, vec.z, 0.0f);

	// クォータニオンの共役を取る
	Quaternion revQ(-x, -y, -z, w);

	// Q * Vq * Q^-1 = v'
	// 純粋クォータニオンが返ってくる
	auto ans = *this * Vq * revQ;

	// 返ってきたクォータニオンをベクトルに変換
	return Vector3(ans.x, ans.y, ans.z);
}