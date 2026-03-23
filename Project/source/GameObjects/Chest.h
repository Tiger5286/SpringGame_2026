#pragma once
#include "GameObject.h"
#include "../System/Animation.h"

class Chest :
    public GameObject
{
public:
    Chest();
    virtual ~Chest() override;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

    void OnCollision(const GameObject& other) override;

private:
    Animation m_anim;

    bool m_isHitPunch = false;
};

