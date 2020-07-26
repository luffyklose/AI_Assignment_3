#pragma once
#include "Sprite.h"
#include "../Template/EnemyAnimationState.h"

const int ENEMYRDAMAGE = 10;

class Enemy:public Sprite
{
public:
	Enemy(float x, float y);
	~Enemy();
	
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void DecHP(int damage);
	int getDetectionRadius();

	void m_buildAnimations();
	void setAnimationState(EnemyAnimationState new_state);
	
protected:
	int m_HealthPoint,
	m_detectionRadius;
	const int m_enemyDamage;
	EnemyAnimationState m_currentAnimationState;
};
