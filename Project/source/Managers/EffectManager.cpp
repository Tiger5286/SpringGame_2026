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
	UpdateEffekseer3D();
	// 再生されていないエフェクトハンドルを除外する
	for (auto& handle : m_effectPlayingHandles)
	{
		if (IsEffekseer3DEffectPlaying(handle) == -1)
		{
			m_effectPlayingHandles.remove(handle);
		}
	}
}

void EffectManager::Draw()
{
	DrawEffekseer3D();
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

void EffectManager::PlayEffect(const std::wstring& name, const Vector3& pos)
{
	// 指定されたエフェクトが存在するかどうかチェック
	if (!(m_effectResourceHandles.contains(name)))
	{
		assert(false && "存在しないエフェクトを再生しようとしています");
		return;
	}
	// エフェクトを再生し正しく再生されたかチェック
	auto handle = PlayEffekseer3DEffect(m_effectResourceHandles[name]);
	assert(handle != -1 && "エフェクトが正しく再生されませんでした");
	// 位置を設定
	SetPosPlayingEffekseer3DEffect(handle, pos.x, pos.y, pos.z);
	// リストに登録
	m_effectPlayingHandles.push_back(handle);
}