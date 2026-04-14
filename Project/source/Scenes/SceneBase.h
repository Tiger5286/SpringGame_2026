#pragma once

class Input;
class SceneManager;

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
	SceneBase(Input& input,SceneManager& sceneManager);
	virtual ~SceneBase();

	virtual void Init() abstract;
	virtual void End() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

	SceneType GetSceneType() const { return m_sceneType; }

protected:
	Input& m_input;
	SceneManager& m_sceneManager;

	SceneType m_sceneType = SceneType::None;
};