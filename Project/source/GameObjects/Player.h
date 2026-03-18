#pragma once
#include "GameObject.h"
#include "../System/Input.h"

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

    void SetCameraAngleY(float angle) { m_cameraAngleY = angle; }

private:
    Input& m_input;

    float m_cameraAngleY = 0.0f;

    float m_angle = 0.0f;
};