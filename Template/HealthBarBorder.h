#pragma once
#include "../src/DisplayObject.h"
#include "../src/Player.h"

class HealthBarBorder: public DisplayObject
{
public:
	HealthBarBorder(DisplayObject* player);
	~HealthBarBorder();
	
	void draw() override;
	void update() override;
	void clean() override;

private:
	DisplayObject* m_pDisplayObject;
};

