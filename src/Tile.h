#pragma once
#include "Sprite.h"
#include "PathNode.h"
#include "Label.h"

class Tile : public Sprite
{
public:
	Tile( float x, float y);
	~Tile();
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;
	
	bool& IsObstacle() { return m_obstacle; }
	bool IsHazard() { return m_hazard; }
	
	PathNode* Node() { return m_node; }
	virtual int getMaxhealth() = 0;
public:
	PathNode* m_node;
protected:
	bool m_obstacle, m_hazard;
};

class Grass: public Tile
{
public:
	Grass(float x, float y);

	Grass* Clone() { return new Grass(getTransform()->position.x, getTransform()->position.y); }

	void draw() override;
	void update() override;
	void clean() override;

	int getMaxhealth() { return grassHP; }
private:
	
public:
	
private:
	const int grassHP = 0;
};

class Brick : public Tile
{
public:
	Brick(float x, float y);
	Brick* Clone() { return new Brick(getTransform()->position.x, getTransform()->position.y); }
	
	void draw() override;
	void update() override;
	void clean() override;

	int getMaxhealth() { return brickHP; }
private:

public:

private:
	const int brickHP = 50;
};