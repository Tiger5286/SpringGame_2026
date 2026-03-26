#include "ChestManager.h"
#include "../GameObjects/Chest.h"
#include "../Managers/ModelManager.h"
#include "../Managers/CollisionManager.h"
#include "../Game.h"


ChestManager::ChestManager(ModelManager& modelManager, CollisionManager& collisionManager, CoinManager& coinManager,EffectManager& effectManager):
	m_modelManager(modelManager),
	m_collisionManager(collisionManager),
	m_coinManager(coinManager),
	m_effectManager(effectManager)
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
	std::list<std::shared_ptr<Chest>> deadChests;
	for (auto& chest : m_chests)
	{
		chest->Update();
		if (chest->IsDead())
		{
			deadChests.push_back(chest);
		}
	}
	for (auto& chest : deadChests)
	{
		m_collisionManager.Unregister(chest);
		chest->End();
		m_chests.remove(chest);
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
	auto newChest = std::make_shared<Chest>(m_coinManager,m_effectManager);
	newChest->SetHandle(m_modelManager.DuplicateModel(L"Chest"));
	newChest->Init();
	Vector3 spawnPos = Vector3(static_cast<float>(GetRand(Game::kFieldSize * 2) - Game::kFieldSize),
		0.0f,
		static_cast<float>(GetRand(Game::kFieldSize * 2) - Game::kFieldSize));
	newChest->Spawn(spawnPos,playerPos);
	m_collisionManager.Register(newChest);

	m_chests.push_back(newChest);
}
