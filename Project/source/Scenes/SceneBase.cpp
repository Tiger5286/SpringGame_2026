#include "SceneBase.h"

SceneBase::SceneBase(Input& input,SceneManager& sceneManager):
	m_input(input),
	m_sceneManager(sceneManager)
{
}

SceneBase::~SceneBase()
{
}
