#include "Enemy.h"

Enemy::Enemy()
{
	
}

Enemy::~Enemy()
= default;

void Enemy::DecHP(int damage)
{
	m_HealthPoint -= damage;
}

int Enemy::getDetectionRadius()
{
	return m_detectionRadius;
}


