#pragma once
#include <map>
#include <string>

class ModelManager
{
public:
	ModelManager();
	virtual ~ModelManager();

	void End();

	void LoadModel(std::wstring fileName,std::wstring key);
	void DeleteModel(std::wstring key);

private:
	bool m_isEnd = false;
	std::map<std::wstring, int> m_modelHandles;
};

