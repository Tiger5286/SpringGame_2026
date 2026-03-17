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

private:
    Input& m_input;
};