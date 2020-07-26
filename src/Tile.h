#pragma once
#include "Sprite.h"

class Tile:public Sprite
{
public:
	Tile(std::string id, float x, float y, bool isObstacle);
	~Tile();

	bool getObstacle() { return m_isObstacle; }
private:
	bool m_isObstacle;
};
