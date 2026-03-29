#pragma once
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Init() abstract;
	virtual void End() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

private:

};

