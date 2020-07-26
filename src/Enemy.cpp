#include "Enemy.h"

#include "TextureManager.h"

Enemy::Enemy(float x, float y):m_currentAnimationState(ENEMY_IDLE_RIGHT), m_enemyDamage(ENEMYRDAMAGE)
{
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/kaben.txt", "../Assets/sprites/kaben.png", "kaben");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("kaben"));

	// set frame width
	// 

	getTransform()->position = glm::vec2(x, y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->maxSpeed = 10.0f;
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	m_HealthPoint = 100;

	m_buildAnimations();
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

void Enemy::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-idle-3"));

	setAnimation(idleAnimation);

	Animation walkAnimation = Animation();

	walkAnimation.name = "walk";
	walkAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-walk-0"));
	walkAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-walk-1"));
	walkAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-walk-2"));
	walkAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-walk-3"));
	walkAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-walk-4"));
	walkAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-walk-5"));
	walkAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-walk-6"));
	walkAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-walk-7"));

	setAnimation(walkAnimation);

	Animation hitAnimation = Animation();
	hitAnimation.name = "hit";
	hitAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-hit-0"));
	hitAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-hit-1"));
	hitAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-hit-2"));
	hitAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-hit-3"));
	hitAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-hit-4"));
	hitAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-hit-5"));
	hitAnimation.frames.push_back(getSpriteSheet()->getFrame("kaben-hit-6"));

	setAnimation(hitAnimation);

	Animation beHitAnimation = Animation();
	beHitAnimation.name = "behit";
}

