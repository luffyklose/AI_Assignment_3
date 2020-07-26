#pragma once
#include "GameObject.h"

class PathNode:public GameObject
{
public:
	PathNode();
	~PathNode();

	virtual void update() override;
	virtual void draw() override;
	virtual void clean() override;

	void setLOS(bool a);
	bool getLOS() const;
private:

public:

private:
	bool m_bLOS;
};
