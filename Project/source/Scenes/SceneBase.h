#pragma once

class Input;

class SceneBase
{
public:
	SceneBase(Input& input);
	virtual ~SceneBase();

	virtual void Init() abstract;
	virtual void End() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

	bool IsEnd() const { return m_isEnd; }

protected:
	Input& m_input;

	bool m_isEnd = false;
};