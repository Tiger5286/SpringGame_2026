#include "HighScoreManager.h"
#include <fstream>
#include "Dxlib.h"
#include <algorithm>

namespace
{
	const wchar_t kDataFileNameWstring[] = L"highScores.data";
	const char kDataFileName[] = "highScores.data";
}

HighScoreManager::HighScoreManager()
{
}

HighScoreManager::~HighScoreManager()
{
}

HighScoreManager& HighScoreManager::GetInstance()
{
	static HighScoreManager instance;
	return instance;
}

void HighScoreManager::Load()
{
	// データをロード
	auto fileName = kDataFileNameWstring;
	int handle = FileRead_open(fileName);

	FileRead_read(&m_highScores, sizeof(m_highScores), handle);
	
	FileRead_close(handle);
}

void HighScoreManager::Save()
{
	// データをセーブ
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, kDataFileName, "wb");
	if (fp == nullptr) return;

	fwrite(&m_highScores, sizeof(m_highScores), 1, fp);
	fclose(fp);
}

void HighScoreManager::RecordScore(int score)
{
	// 新しいスコアを挿入すべき位置を探す
	auto it = std::lower_bound(m_highScores.begin(), m_highScores.end(), score, std::greater<int>());

	// もし挿入位置がランキングの範囲内なら更新
	if (it != m_highScores.end())
	{
		// 挿入位置から後ろの要素を一つずつ後ろにずらす(最後の要素は消える)
		std::copy_backward(it, std::prev(m_highScores.end()), m_highScores.end());
		// 新しいスコアを適切な位置に格納
		*it = score;
	}
}