#pragma once
#include "GameObject.h"
#include "../System/Input.h"
#include <memory>

class PunchCollider;
class CollisionManager;

enum class PlayerState
{
    Idle,
    Move,
    Punch,
};

class Player :
    public GameObject
{
public:
    Player(Input& input,CollisionManager& collisionManager);
    virtual ~Player() override;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

	void OnCollision(const GameObject& other) override;

    void SetCameraAngleY(float angle) { m_cameraAngleY = angle; }

private:
    void Move();
    void Punch();
    void UpdateState();
    void UpdateAnimation();

    // ステートが変わった瞬間を取得する関数
    bool TriggeredChangeState(PlayerState state) const;

private:
    Input& m_input;
	CollisionManager& m_collisionManager;

	PlayerState m_state = PlayerState::Idle;
	PlayerState m_prevState = PlayerState::Idle;

    Animation m_animation;

    Vector3 m_prevPos;

	std::shared_ptr<PunchCollider> m_pPunchCollider = nullptr;

    float m_cameraAngleY = 0.0f;

    float m_angle = 0.0f;

    int m_punchFrame = 0;
};