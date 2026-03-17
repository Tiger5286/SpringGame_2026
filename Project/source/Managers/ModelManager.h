#pragma once
#include <map>
#include <string>
#include <list>

/// <summary>
/// モデルを管理するクラス
/// </summary>
class ModelManager
{
public:
	ModelManager();
	virtual ~ModelManager();

	void End();

	/// <summary>
	/// モデルをロードし登録する
	/// </summary>
	/// <param name="fileName">ロードするモデルのファイル名</param>
	/// <param name="key">登録するモデルの名前</param>
	void LoadModel(std::wstring fileName,std::wstring key);

	/// <summary>
	/// モデルを削除し登録解除する
	/// </summary>
	/// <param name="key">削除するモデルの名前</param>
	void DeleteModel(std::wstring key);

	/// <summary>
	/// 登録されたモデルを複製しそのハンドルを返す
	/// このクラスは複製されたモデルのハンドルを管理しないのでそれぞれで管理すること
	/// </summary>
	/// <param name="key">複製するモデルの名前</param>
	/// <returns>複製されたモデルのハンドル</returns>
	int DuplicateModel(std::wstring key);

private:
	bool m_isEnd = false;
	std::map<std::wstring, int> m_modelHandles;
};

