#pragma once
#include "GameObject.h"
#include "../System/Animation.h"

class CoinManager;

class Chest :
    public GameObject
{
public:
    Chest(CoinManager& coinManager);
    virtual ~Chest() override;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

    void OnCollision(const GameObject& other) override;

private:
    CoinManager& m_coinManager;

    Animation m_anim;

    bool m_isHitPunch = false;
    int m_hitPunchFrame = 0;

    int m_spawnCoinCount = 0;

    bool m_isDead = false;
};

