#include "Player.h"
#include "TextureManager.h"

Player::Player(float x,float y): m_currentAnimationState(PLAYER_IDLE_RIGHT),m_playerDamage(PLAYERDAMAGE)
{
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/kaben.txt", "../Assets/sprites/kaben.png", "kaben");

	//setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("kaben"));
	
	// set frame width
	//setWidth(53);

	//// set frame height
	//setHeight(58);

	getTransform()->position = glm::vec2(x,y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->maxSpeed = 10.0f;
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	m_HealthPoint = 100;

	
	
	m_buildAnimations();
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance()->playAnimation("kaben", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("kaben", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_RIGHT:
		TextureManager::Instance()->playAnimation("kaben", getAnimation("walk"),
			x, y, 0.12f, 0, 255, true);
		break;
	case PLAYER_RUN_LEFT:
		TextureManager::Instance()->playAnimation("kaben", getAnimation("walk"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_HIT_LEFT:
		TextureManager::Instance()->playAnimation("kaben", getAnimation("hit"),
			x, y, 0.25f, 0, 255, true);
	case PLAYER_HIT_RIGHT:
		TextureManager::Instance()->playAnimation("kaben", getAnimation("hit"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		
	default:
		break;
	}
	
}

void Player::update()
{
}

void Player::clean()
{
}

void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::m_buildAnimations()
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
}

int Player::getDamage()
{
	return m_playerDamage;
}
