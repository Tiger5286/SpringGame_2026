#pragma once
#include "GameObject.h"

class Player;

class Enemy :
    public GameObject
{
public:
    Enemy(Player& player);
    virtual ~Enemy() override;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

	void OnCollision(const GameObject& other) override;

    void Spawn(const Vector3& spawnPos);

	void OnHitPunch();

	bool IsDead() const { return m_isDead; }

private:
    void Move();

    /// <summary>
    /// 他のオブジェクトとぶつかったときの押し戻し処理
    /// </summary>
    /// <param name="center">ぶつかったオブジェクトの球の中心</param>
    /// <param name="radius">ぶつかったオブジェクトの球の半径</param>
    void PushBack(const Vector3& center, float radius);

private:
    Player& m_player;

    int m_animHandle = -1;
    float m_animTime = 0.0f;

    bool m_isHitPunch = false;
	int m_hitPunchFrame = 0;
	bool m_isDead = false;
};

