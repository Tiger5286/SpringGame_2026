#pragma once
#include "GameObject.h"
#include "../System/Animation.h"

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
	bool IsHitPunch() const { return m_isHitPunch; }

	int GetFrameCount() const { return m_frameCount; }
private:
    void Move();

    /// <summary>
    /// 他のオブジェクトとぶつかったときの押し戻し処理
    /// </summary>
    /// <param name="center">ぶつかったオブジェクトの球の中心</param>
    /// <param name="radius">ぶつかったオブジェクトの球の半径</param>
    void PushBack(const Vector3& center, float radius);

private:
    int m_frameCount = 0;

    Player& m_player;

    Animation m_anim;

    bool m_isHitPunch = false;
	int m_hitPunchFrame = 0;
	bool m_isDead = false;
};

