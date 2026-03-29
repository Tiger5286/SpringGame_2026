#pragma once

class Input;

enum class SceneType
{
	None,
	Title,
	Main,
	Result,
};

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
	SceneType GetSceneType() const { return m_sceneType; }

protected:
	Input& m_input;
	SceneType m_sceneType = SceneType::None;
	bool m_isEnd = false;
};