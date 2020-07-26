#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "Obstacle.h"
#include "PathNode.h"

const int MAXENEMY = 1;

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	void buildGrid();
	void displayGrid();
	void drawLOS();
	void setGridLOS();
public:
	
private:	
	glm::vec2 m_mousePosition;

	//Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Enemy* m_pEnemy;
	
	bool m_playerFacingRight;
	bool m_isDebugMode;

	bool m_pHPressed,
		m_pPPressed,
		m_pKPressed;

	//Obstacle* m_pObstacle;
	std::vector<PathNode*> m_pathNodeVec;
	/*std::vector<Enemy*> m_enemyVec;*/
	std::vector<Obstacle*> m_obstacleVec;

	//UI Labels
	Label* m_pScoreLabel;
	Label* m_pEnemyKilledLabel;

	int m_enemyKilled{};
};

#endif /* defined (__PLAY_SCENE__) */