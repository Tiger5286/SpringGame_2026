#pragma once
#include "GameObject.h"
#include "../System/Input.h"

enum class PlayerState
{
    Idle,
    Move,
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

private:
    Input& m_input;

	PlayerState m_state = PlayerState::Idle;
	PlayerState m_prevState = PlayerState::Idle;

    Animation m_animation;

    float m_cameraAngleY = 0.0f;

    float m_angle = 0.0f;
};