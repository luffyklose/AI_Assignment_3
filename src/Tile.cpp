#include "Tile.h"

Tile::Tile(std::string id,float x, float y, bool isObstacle)
{
	
	setWidth(40);
	setHeight(40);
	m_isObstacle = isObstacle;
	getTransform()->position = glm::vec2(x,y);

	setType(TILE);
}

Tile::~Tile()
{
}
