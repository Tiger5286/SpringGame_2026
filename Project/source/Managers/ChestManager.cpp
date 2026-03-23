#include "ChestManager.h"
#include "../GameObjects/Chest.h"
#include "../Managers/ModelManager.h"
#include "../Game.h"


ChestManager::ChestManager(ModelManager& modelManager, CoinManager& coinManager):
	m_modelManager(modelManager),
	m_coinManager(coinManager)
{
}

ChestManager::~ChestManager()
{
}

void ChestManager::Init()
{
}

void ChestManager::End()
{
	for (auto& chest : m_chests)
	{
		chest->End();
	}
}

void ChestManager::Update()
{
	for (auto& chest : m_chests)
	{
		chest->Update();
	}
}

void ChestManager::Draw()
{
	for (auto& chest : m_chests)
	{
		chest->Draw();
	}
}

void ChestManager::Spawn(const Vector3& playerPos)
{
	auto newChest = std::make_shared<Chest>(m_coinManager);
	newChest->SetHandle(m_modelManager.DuplicateModel(L"Chest"));
	newChest->Init();
	Vector3 spawnPos = Vector3(static_cast<float>(GetRand(Game::kFieldSize * 2) - Game::kFieldSize),
		0.0f,
		static_cast<float>(GetRand(Game::kFieldSize * 2) - Game::kFieldSize));
	newChest->Spawn(spawnPos,playerPos);
}
