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
	
	std::vector<PathConnection*> getPath();
	void getDir();
	
private:
	void m_buildAnimations();
	void MovePlane();
	void SetNextNode();
	//void setPath(std::vector<PathConnection*> path);
public:

private:
	
};

#endif /* defined (__PLANE__) */
