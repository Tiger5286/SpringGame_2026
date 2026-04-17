#pragma once
#include <array>

class HighScoreManager
{
public:
	static HighScoreManager& GetInstance();
	virtual ~HighScoreManager();

	HighScoreManager(const HighScoreManager&) = delete;
	HighScoreManager& operator=(const HighScoreManager&) = delete;
private:
	HighScoreManager();
	static constexpr int kSaveScoreNum = 10;

public:
	void Load();
	void Save();

	void RecordScore();

	const std::array<int, kSaveScoreNum>& GetHighScores() { return m_highScores; }
private:

	std::array<int, kSaveScoreNum> m_highScores = { 0 };
};

