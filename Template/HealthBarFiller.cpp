#include "HealthBarFiller.h"

#include "../src/TextureManager.h"

PlayerHealthBarFiller::PlayerHealthBarFiller(Player* player)
{
	this->m_pPlayer = player;

	TextureManager::Instance()->load("../Assets/textures/HealthBarFiller.png", "HealthBarFiller");

	const auto size = TextureManager::Instance()->getTextureSize("HealthBarFiller");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position.x = m_pPlayer->getTransform()->position.x;
	getTransform()->position.y = m_pPlayer->getTransform()->position.y - 40;
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(PLAYERHEALTHBAR);
}

PlayerHealthBarFiller::~PlayerHealthBarFiller()
= default;

void PlayerHealthBarFiller::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("obstacle", x, y, 0, 255, true);
}

void PlayerHealthBarFiller::update()
{
	const auto size = TextureManager::Instance()->getTextureSize("PlayerHealthBarFiller");

	getTransform()->position.x = m_pPlayer->getTransform()->position.x;
	getTransform()->position.y = m_pPlayer->getTransform()->position.y - 40;

	
}

void PlayerHealthBarFiller::clean()
{

}

