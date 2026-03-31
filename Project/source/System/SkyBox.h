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
	void DrawSkyQuad(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, int graphHandle);

private:
	int m_graphHandles[6] = { -1 };

	Camera& m_camera;
};

