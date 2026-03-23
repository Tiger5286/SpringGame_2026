#pragma once
#include "GameObject.h"
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

};

