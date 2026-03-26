#include "EffectManager.h"
#include "EffekseerForDXLib.h"

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Init()
{
}

void EffectManager::End()
{
}

void EffectManager::Update()
{
}

void EffectManager::Draw()
{
}

void EffectManager::LoadEffect(const std::wstring& filePath, const std::wstring& name, float scale)
{
	if (m_effectResourceHandles.contains(name))
	{
		assert(false && "登録するエフェクトの名前が重複しています");
		return;
	}
	auto handle = LoadEffekseerEffect(filePath.c_str(), scale);
	assert(handle != -1 && "エフェクトが正しくロードされませんでした");
	m_effectResourceHandles[name] = handle;
}