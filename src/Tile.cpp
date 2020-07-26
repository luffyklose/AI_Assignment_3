#include "Tile.h"

#include "TextureManager.h"

Tile::Tile( float x,float y)
{	
	setWidth(40);
	setHeight(40);
	
	getTransform()->position = glm::vec2(x,y);
	m_node = nullptr;
	setType(TILE);
}

Tile::~Tile()
{
	delete m_node; // Will also invoke PathNode destructor. Deletes connections.
}


Grass::Grass(float x, float y):Tile(x,y)
{
	TextureManager::Instance()->load("../Assets/textures/grass.png", "grass");

	auto size = TextureManager::Instance()->getTextureSize("grass");

	m_obstacle = false;
	m_hazard = false;
}

void Grass::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("grass", x , y , getWidth(), getHeight(), 0, 255, false);
}

void Grass::update()
{
}

void Grass::clean()
{
}

Brick::Brick(float x, float y) :Tile(x, y)
{
	TextureManager::Instance()->load("../Assets/textures/brick.png", "brick");

	auto size = TextureManager::Instance()->getTextureSize("brick");

	m_obstacle = true;
	m_hazard = false;
}

void Brick::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("brick", x , y , getWidth(), getHeight(), 0, 255, false);
}

void Brick::update()
{
}

void Brick::clean()
{
}
