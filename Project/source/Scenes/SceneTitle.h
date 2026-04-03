#pragma once
#include "SceneBase.h"
#include <memory>

class Camera;
class SkyBox;

class SceneTitle : public SceneBase
{
public:
	SceneTitle(Input& input);
	virtual ~SceneTitle();

	virtual void Init() override;
	virtual void End() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	int m_titleFontHandle = -1;
	int m_fontHandle = -1;

	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<SkyBox> m_pSkyBox;
};

