#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Enemy.h"

class Plane final : public Enemy
{
public:
	Plane(float x,float y);
	~Plane();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	
private:
	void m_buildAnimations();
};

#endif /* defined (__PLANE__) */
