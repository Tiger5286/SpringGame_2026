#pragma once
#include <string>
#include <list>
#include <map>

class EffectManager
{
public:
	EffectManager();
	virtual ~EffectManager();

	void Init();
	void End();
	void Update();
	void Draw();

	/// <summary>
	/// エフェクトをロードし登録する
	/// </summary>
	/// <param name="filePath">ロードするエフェクトのファイルパス</param>
	/// <param name="name">登録する名前</param>
	/// <param name="scale">エフェクトの拡大率</param>
	void LoadEffect(const std::wstring& filePath, const std::wstring& name,float scale = 1.0f);

private:
	std::map<std::wstring, int> m_effectResourceHandles;
	std::list<int> m_effectPlayingHandles;
};