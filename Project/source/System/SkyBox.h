#pragma once
class SkyBox
{
public:
	SkyBox();
	virtual ~SkyBox();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	int m_graphHandle[6] = { -1 };
};

