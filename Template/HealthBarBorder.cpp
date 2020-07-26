#include "HealthBarBorder.h"

#include "../src/TextureManager.h"

HealthBarBorder::HealthBarBorder(DisplayObject* player)
{
	this->m_pDisplayObject = player;

	TextureManager::Instance()->load("../Assets/textures/HealthBarFiller.png", "HealthBarBorder");

	const auto size = TextureManager::Instance()->getTextureSize("HealthBarBorder");
	setWidth(size.x / 2);
	setHeight(size.y);
	getTransform()->position.x = m_pDisplayObject->getTransform()->position.x;
	getTransform()->position.y = m_pDisplayObject->getTransform()->position.y - 40;
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;
	
	setType(PLAYERHEALTHBAR);
}

HealthBarBorder::~HealthBarBorder()
= default; 

void HealthBarBorder::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("obstacle", x, y, 0, 255, true);
}

void HealthBarBorder::update()
{
	const auto size = TextureManager::Instance()->getTextureSize("obstacle");
	getTransform()->position.x = m_pDisplayObject->getTransform()->position.x;
	getTransform()->position.y = m_pDisplayObject->getTransform()->position.y - 40;
}

void HealthBarBorder::clean()
{
	
}

