#pragma once
#include "GameObject.h"
#include "../System/Input.h"
#include "../System/Animation.h"
#include <memory>

class PunchCollider;
class CollisionManager;
class EffectManager;

enum class PlayerState
{
    Idle,
    Move,
    Punch,
    Hit,
};

class Player :
    public GameObject
{
public:
    Player(Input& input, std::shared_ptr<CollisionManager> pCollisionManager,std::shared_ptr<EffectManager> pEffectManager);
    virtual ~Player() override;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

	void OnCollision(const GameObject& other) override;

	// カメラの角度をプレイヤーに渡す関数
    void SetCameraAngleY(float angle) { m_cameraAngleY = angle; }

	// プレイヤーの操作ができるかどうかを設定する関数
	void SetCanControll(bool isCanControll) { m_isCanControll = isCanControll; }

	// プレイヤーの無敵状態を設定する関数
    void SetInvincible(bool isInvincible);

	// プレイヤーの位置をタイトルシーンでの初期位置に設定する関数
    void SetFirstTitlePos();

private:
    void Move();
    void Punch();
    void PunchNoCollider();
    void HitUpdate();
    void UpdateState();
    void UpdateAnimation();

    // ステートが変わった瞬間を取得する関数
    bool TriggeredChangeState(PlayerState state) const;

private:
    Input& m_input;
	std::shared_ptr<CollisionManager> m_pCollisionManager;
	std::shared_ptr<EffectManager> m_pEffectManager;

	PlayerState m_state = PlayerState::Idle;
	PlayerState m_prevState = PlayerState::Idle;

    Animation m_animation;

    Vector3 m_prevPos;

	std::shared_ptr<PunchCollider> m_pPunchCollider = nullptr;

    bool m_isCanControll = true;

    float m_cameraAngleY = 0.0f;

    float m_angle = 0.0f;

    int m_punchFrame = 0;

    bool m_isHitEnemy = false;
    int m_hitFrame = 0;

    bool m_isInvincible = false;
    // 負の数にすると無限に無敵になる
    int m_invincibleFrame = 0;
};