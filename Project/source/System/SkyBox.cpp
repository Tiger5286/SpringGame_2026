#include "SkyBox.h"
#include <string>
#include "Dxlib.h"
#include <cassert>

#include "../Utility/Vector3.h"
#include "../System/Camera.h"

namespace
{
	struct SkyboxData
	{
		std::wstring filePath;	// ファイルパス
		std::wstring key;		// 登録名
	};

	SkyboxData kSkyboxData[] =
	{
		L"data/Graphs/skybox_up.png",    L"SkyboxUp",
		L"data/Graphs/skybox_down.png",  L"SkyboxDown",
		L"data/Graphs/skybox_left.png",  L"SkyboxLeft",
		L"data/Graphs/skybox_right.png", L"SkyboxRight",
		L"data/Graphs/skybox_front.png", L"SkyboxFront",
		L"data/Graphs/skybox_back.png",  L"SkyboxBack"
	};
}

SkyBox::SkyBox(Camera& camera):
	m_camera(camera)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Init()
{
	for (int i = 0; i < 6; i++)
	{
		m_graphHandles[i] = LoadGraph(kSkyboxData[i].filePath.c_str());
		assert(m_graphHandles[i] != -1 && "スカイボックスの画像が正しく読み込まれませんでした");
	}
}

void SkyBox::End()
{
	for (auto& handle : m_graphHandles)
	{
		DeleteGraph(handle);
	}
}

void SkyBox::Update()
{
}

void SkyBox::Draw()
{
	float size = 800.0f;

	Vector3 cPos = m_camera.GetPos();

	Vector3 v[8] =
	{
		{ cPos.x - size, cPos.y + size,cPos.z - size},
		{ cPos.x + size, cPos.y + size,cPos.z - size},
		{ cPos.x + size, cPos.y - size,cPos.z - size},
		{ cPos.x - size, cPos.y - size,cPos.z - size},

		{ cPos.x - size, cPos.y + size,cPos.z + size},
		{ cPos.x + size, cPos.y + size,cPos.z + size},
		{ cPos.x + size, cPos.y - size,cPos.z + size},
		{ cPos.x - size, cPos.y - size,cPos.z + size}
	};

	// 前
	DrawSkyQuad(v[5], v[4], v[7], v[6], m_graphHandles[4]);

	// 後
	DrawSkyQuad(v[0], v[1], v[2], v[3], m_graphHandles[5]);

	// 左
	DrawSkyQuad(v[4], v[0], v[3], v[7], m_graphHandles[2]);

	// 右
	DrawSkyQuad(v[1], v[5], v[6], v[2], m_graphHandles[3]);

	// 上
	DrawSkyQuad(v[4], v[5], v[1], v[0], m_graphHandles[0]);

	// 下
	DrawSkyQuad(v[3], v[2], v[6], v[7], m_graphHandles[1]);
}

void SkyBox::DrawSkyQuad(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, int graphHandle)
{
	VERTEX3D v[4];

	v[0].pos = a.ToDxLib(); v[0].u = 0.0f; v[0].v = 0.0f;
	v[1].pos = b.ToDxLib(); v[1].u = 1.0f; v[1].v = 0.0f;
	v[2].pos = c.ToDxLib(); v[2].u = 1.0f; v[2].v = 1.0f;
	v[3].pos = d.ToDxLib(); v[3].u = 0.0f; v[3].v = 1.0f;

	for (int i = 0; i < 4; i++)
	{
		v[i].dif.r = 255;
		v[i].dif.g = 255;
		v[i].dif.b = 255;
		v[i].dif.a = 255;

		v[i].spc.r = 0;
		v[i].spc.g = 0;
		v[i].spc.b = 0;
		v[i].spc.a = 0;
	}

	DrawPolygon3D(&v[0], 3, graphHandle, false);

	VERTEX3D v2[3] = { v[0], v[2], v[3] };
	DrawPolygon3D(v2, 3, graphHandle, false);
}