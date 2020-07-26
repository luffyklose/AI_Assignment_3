#pragma once

#include "../src/DisplayObject.h"
#include "../src/Player.h"

class PlayerHealthBarFiller : public DisplayObject
{
public:
	PlayerHealthBarFiller(Player* player);
	~PlayerHealthBarFiller();
	void draw() override;
	void update() override;
	void clean() override;

private:
	Player* m_pPlayer;

};