#pragma once
#include "GameObject.h"
#include "../System/Animation.h"

class CoinManager;
class EffectManager;

class Chest :
    public GameObject
{
public:
    Chest(CoinManager& coinManager,EffectManager& effectManager);
    virtual ~Chest() override;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

    void OnCollision(const GameObject& other) override;

    void Spawn(const Vector3& spawnPos, const Vector3& targetPos);

    bool IsDead() const { return m_isDead; }

private:
    CoinManager& m_coinManager;
    EffectManager& m_effectManager;

    Animation m_anim;

    float m_angle = 0.0f;

    bool m_isHitPunch = false;
    int m_hitPunchFrame = 0;

    int m_spawnCoinCount = 0;

    bool m_isDead = false;
};

