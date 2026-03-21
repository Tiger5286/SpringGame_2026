#pragma once
#include "GameObject.h"
#include "../System/Input.h"

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
    Player(Input& input);
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

	PlayerState m_state = PlayerState::Idle;
	PlayerState m_prevState = PlayerState::Idle;

    Animation m_animation;

    Vector3 m_prevPos;

    float m_cameraAngleY = 0.0f;

    float m_angle = 0.0f;

    int m_punchFrame = 0;
};