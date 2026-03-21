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

private:
    float m_angle = 0.0f;
};

