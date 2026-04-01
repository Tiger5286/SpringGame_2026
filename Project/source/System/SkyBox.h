#pragma once

class Vector3;
class Camera;

class SkyBox
{
public:
	SkyBox(Camera& camera);
	virtual ~SkyBox();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	void DrawSquare(const Vector3& lt, const Vector3& rt, const Vector3& rb, const Vector3& lb, int graphHandle);

private:
	enum class SkyBoxFace
	{
		Front,
		Back,
		Left,
		Right,
		Up,
		Down,

		Num
	};

private:
	int m_graphHandles[static_cast<int>(SkyBoxFace::Num)] = { -1 };

	Camera& m_camera;
};

