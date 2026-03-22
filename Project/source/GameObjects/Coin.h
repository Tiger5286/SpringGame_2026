#pragma once
#include "GameObject.h"
class Coin :
    public GameObject
{
public:
    Coin();
    virtual ~Coin() override;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

	void OnCollision(const GameObject& other) override;

	void Spawn(const Vector3& pos);

    bool IsHit() const { return m_isHitPlayer; }
    bool IsDead() const { return m_isDead; }

private:
    float m_angle = 0.0f;

    bool m_isHitPlayer = false;
    bool m_isDead = false;
};

