#include "ModelManager.h"
#include "Dxlib.h"
#include <cassert>

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
	assert(m_isEnd && "ModelManager::End()が呼ばれていません。インスタンスを削除する前にEnd()を呼んでください。");
}

void ModelManager::End()
{
	// すべてのロードしたモデルを削除
	for (auto& handle : m_modelHandles)
	{
		this->DeleteModel(handle.first);
	}
	m_isEnd = true;
}

void ModelManager::LoadModel(std::wstring fileName, std::wstring key)
{
	// キー重複チェック
	if (m_modelHandles.contains(key))
	{
		assert("同じ名前のモデルをロードしようとしています");
		return;
	}
	// モデルをロードして登録
	auto handle = MV1LoadModel(fileName.c_str());
	assert(handle != -1 && "モデルが正しくロードされませんでした");
	m_modelHandles[key] = handle;
}

void ModelManager::DeleteModel(std::wstring key)
{
	// 存在しないキーをチェック
	if (!m_modelHandles.contains(key))
	{
		assert("存在しないモデルを削除しようとしています");
		return;
	}
	// モデルを削除してキーを除外
	auto check = MV1DeleteModel(m_modelHandles[key]);
	assert(check != -1 && "モデルが正しく削除されませんでした");
	m_modelHandles.erase(key);
}