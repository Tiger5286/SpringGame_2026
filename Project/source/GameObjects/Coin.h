#pragma once
#include "GameObject.h"

class CoinManager;

class Coin :
    public GameObject
{
public:
    Coin(CoinManager& coinManager);
    virtual ~Coin() override;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

	void OnCollision(const GameObject& other) override;

	void Spawn(const Vector3& pos);

	// プレイヤーの位置を設定する(Updateの前に毎フレーム呼ぶ)
	void SetPlayerPos(const Vector3& pos) { m_playerPos = pos; }

	// 引き寄せを発動させる
    void ActivateAtract(const Vector3& playerPos);

    bool IsHit() const { return m_isHitPlayer; }
    bool IsDead() const { return m_isDead; }
    /// <summary>
    /// 取得された瞬間だけtrueを返す
    /// </summary>
    bool IsGetted() const { return m_hitFrame == 1; }
    int GetAliveFrame() const { return m_aliveFrame; }

private:
    CoinManager& m_coinManager;

	Vector3 m_playerPos; // プレイヤーの位置

    float m_angle = 0.0f;

    int m_aliveFrame = 0;
    int m_hitFrame = 0;

    bool m_isHitPlayer = false;
    bool m_isDead = true;

	bool m_isAtract = false; // 引き寄せが発動しているか
    float m_toPlayerSquareDist = 0.0f;  // 引き寄せ時に使うプレイヤーとの距離の2乗
};