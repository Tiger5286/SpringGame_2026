#include "Player.h"

Player::Player(Input& input) :
	m_input(input)
{
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::End()
{
}

void Player::Update()
{
	
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}