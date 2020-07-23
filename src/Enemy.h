#pragma once
#include "Sprite.h"

class Enemy:public Sprite
{
public:
	Enemy();
	~Enemy();
	
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;

	void DecHP(int damage);
	int getDetectionRadius();
protected:
	int m_HealthPoint,
	m_detectionRadius;
};
