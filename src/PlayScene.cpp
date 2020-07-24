#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	if(m_isDebugMode)
	{
		Util::DrawRect(m_pPlayer->getTransform()->position - glm::vec2(m_pPlayer->getWidth() * 0.5f, m_pPlayer->getHeight() * 0.5f), m_pPlayer->getWidth(), m_pPlayer->getHeight());

		for(auto m_pEnemy:m_enemyVec)
		{
			Util::DrawLine(m_pPlayer->getTransform()->position, m_pEnemy->getTransform()->position);
			Util::DrawRect(m_pEnemy->getTransform()->position - glm::vec2(m_pEnemy->getWidth() * 0.5f, m_pEnemy->getHeight() * 0.5f), m_pEnemy->getWidth(), m_pEnemy->getHeight());
			Util::DrawCircle(m_pEnemy->getTransform()->position + glm::vec2(m_pEnemy->getWidth() * 0.5f, m_pEnemy->getHeight() * 0.5f), m_pEnemy->getDetectionRadius());
		}	

		for(auto m_pObstacle:m_obstacleVec)
		{
			Util::DrawRect(m_pObstacle->getTransform()->position - glm::vec2(m_pObstacle->getWidth() * 0.5f, m_pObstacle->getHeight() * 0.5f), m_pObstacle->getWidth(), m_pObstacle->getHeight());

		}

		displayGrid();
		
		drawLOS();
	}
	
}

void PlayScene::update()
{
	updateDisplayList();

	for(auto m_pEnemy:m_enemyVec)
	{
		for(auto m_pObstacle:m_obstacleVec)
		{
			CollisionManager::LOSCheck(m_pPlayer, m_pEnemy, m_pObstacle);
		}
	}
	
	setGridLOS();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;

				m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;

				m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
		}
		else if(EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, -5.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
		}
		else if(EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, 5.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_J))
		{
			SDL_Rect temp;
			
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_HIT_LEFT);
				std::cout << "hit left!" << std::endl;
				temp.x = m_pPlayer->getTransform()->position.x + m_pPlayer->getTransform()->position.x;
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_HIT_RIGHT);
				std::cout << "hit right!" << std::endl;
				temp.x = m_pPlayer->getTransform()->position.x + m_pPlayer->getTransform()->position.x;
			}

			temp.y = m_pPlayer->getTransform()->position.y;
			temp.w = m_pPlayer->getTransform()->position.x;
			temp.h = m_pPlayer->getTransform()->position.g;
		}

		
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}

	if(!m_pHPressed)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
		{
			m_isDebugMode=!m_isDebugMode;
			m_pHPressed = true;
			if(m_isDebugMode)
			{
				std::cout << "Enter Debug Mode" << std::endl;
			}
			else
			{
				std::cout << "Quit Debug Mode" << std::endl;
			}
		}
	}
	
	if(EventManager::Instance().isKeyUp(SDL_SCANCODE_H))
	{
		m_pHPressed = false;
	}

	if (!m_pPPressed)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
		{
			m_pPPressed = true;
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P))
	{
		m_pPPressed = false;
		for(auto m_pEnemy:m_enemyVec)
		{
			m_pEnemy->DecHP(m_pPlayer->getDamage());
		}
	}

	if (!m_pKPressed)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
		{
			m_pKPressed = true;
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_K))
	{
		m_pKPressed = false;
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::buildGrid()
{
	for(int row=0;row<Config::ROW_NUM;++row)
	{
		for(int col=0;col<Config::COL_NUM;++col)
		{
			PathNode* tempNode = new PathNode();
			tempNode->getTransform()->position = glm::vec2(tempNode->getWidth() * col+0.5f*Config::TILE_SIZE, tempNode->getHeight() * row+0.5f * Config::TILE_SIZE);
			m_pathNodeVec.push_back(tempNode);
		}
	}
}

void PlayScene::displayGrid()
{
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Util::DrawRect(m_pathNodeVec[row * Config::COL_NUM + col]->getTransform()->position - glm::vec2(m_pathNodeVec[row * Config::COL_NUM + col]->getWidth() * 0.5f, m_pathNodeVec[row * Config::COL_NUM + col]->getHeight() * 0.5f), 40, 40);
			Util::DrawRect(m_pathNodeVec[row * Config::COL_NUM + col]->getTransform()->position , 5, 5);
		}
	}
}

void PlayScene::setGridLOS()
{
	for(auto node:m_pathNodeVec)
	{
		for(auto m_pObstacle:m_obstacleVec)
		{
			node->setLOS(CollisionManager::LOSCheck(node, m_pPlayer, m_pObstacle));
		}		
	}
}


void PlayScene::drawLOS()
{	
	for (auto node : m_pathNodeVec)
	{
		if(!node->getLOS())
		{
			Util::DrawLine(m_pPlayer->getTransform()->position, node->getTransform()->position, glm::vec4(1, 0, 0, 0));
		}
		else
		{
			Util::DrawLine(m_pPlayer->getTransform()->position, node->getTransform()->position,glm::vec4(0,0,1,0));
		}			
	}
}


void PlayScene::start()
{
	buildGrid();
	// Plane Sprite
	//m_pPlaneSprite = new Plane(400.0f,300.0f);
	m_enemyVec.push_back(new Plane(400.0f, 100.0f));
	for (auto m_pEnemy : m_enemyVec)
	{
		addChild(m_pEnemy);
		std::cout << "Position: " << m_pEnemy->getTransform()->position.x << " " << m_pEnemy->getTransform()->position.y << std::endl;
	}
	std::cout << "Enemy: " << (int)m_enemyVec.size() << " "<< numberOfChildren()<<std::endl;
	
	

	// Player Sprite
	m_pPlayer = new Player(600.0f,500.0f);
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Obstacle Texture
	//m_pObstacle = new Obstacle();
	m_obstacleVec.push_back(new Obstacle(400.0f, 300.0f));
	for(auto m_pObstacle:m_obstacleVec)
	{
		addChild(m_pObstacle);
	}
	

	m_isDebugMode = false;
	m_pHPressed = false;
}
