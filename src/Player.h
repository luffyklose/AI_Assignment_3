#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"

const int PLAYERDAMAGE = 10;

class Player final : public Sprite
{
public:
	Player(float x,float y);
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;



	// setters
	void setAnimationState(PlayerAnimationState new_state);

	//getters
	int getDamage();

private:
	void m_buildAnimations();

	PlayerAnimationState m_currentAnimationState;

	const int m_playerDamage;
};

#endif /* defined (__PLAYER__) */