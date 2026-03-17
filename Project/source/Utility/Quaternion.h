#pragma once
#include "Vector3.h"

/// <summary>
/// クォータニオンを表すクラス
/// </summary>
class Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	virtual ~Quaternion();

	void DrawAxis();

	float x, y, z, w;

	// 軸と角度からクォータニオンを作成する関数
	// 角度は度数法(オイラー角)で指定する
	static Quaternion CreateQuaternion(Vector3 axis, float eulerAngle);

	// クォータニオン同士の乗算
	Quaternion operator*(const Quaternion& other) const;
	void operator*=(const Quaternion& other);
	// クォータニオンとベクトルの乗算
	Vector3 operator*(const Vector3& vec) const;
};

