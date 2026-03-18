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

private:
    void Move();

private:
    Player& m_player;

    int m_animHandle = -1;
    float m_animTime = 0.0f;
};

