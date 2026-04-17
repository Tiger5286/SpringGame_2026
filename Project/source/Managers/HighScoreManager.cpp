#include "HighScoreManager.h"
#include <fstream>
#include "Dxlib.h"

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

void HighScoreManager::RecordScore()
{
	m_highScores[0] = 10000;
}