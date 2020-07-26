#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include"PathManager.h"
#include <fstream>

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
		
		//drawLOS();

		//DrawLine();
		//DrawPath();
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
			for (auto enemy : m_enemyVec)
			{
				enemy->setPatrolMode(!enemy->getPatrolMode());
			}
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P))
	{
		m_pPPressed = false;
		
		
	}

	if (!m_pKPressed)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
		{
			m_pKPressed = true;
			for (auto m_pEnemy : m_enemyVec)
			{
				m_pEnemy->DecHP(m_pPlayer->getDamage());
			}
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
	for(int i=0;i<(int)m_pathNodeVec.size();i++)
	{
		auto GridColor = (!m_pathNodeVec[i]->getLOS()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		//std::cout << "Path: " << m_pathNodeVec.size() << " Num: " << row * Config::COL_NUM + col << std::endl;
		//std::cout << "Grid: " << m_pathNodeVec[i]->getTransform()->position.x - m_pathNodeVec[i]->getWidth() * 0.5f << " " << m_pathNodeVec[i]->getTransform()->position.y - m_pathNodeVec[i]->getHeight() * 0.5f << std::endl;
		Util::DrawRect(m_pathNodeVec[i]->getTransform()->position - glm::vec2(m_pathNodeVec[i]->getWidth() * 0.5f, m_pathNodeVec[i]->getHeight() * 0.5f), 40, 40);
		Util::DrawRect(m_pathNodeVec[i]->getTransform()->position , 5, 5, GridColor);
	}
}

void PlayScene::LoadMap()
{
	std::ifstream inFile("../Assets/data/level.txt");
	if (inFile.is_open())
	{ // Build the level from Tile prototypes.
		char key;
		for (int row = 0; row < Config::ROW_NUM; row++)
		{
			for (int col = 0; col < Config::COL_NUM; col++)
			{
				inFile >> key;
				switch (key)
				{
				case 'g':
					{
					m_level[row][col] = new Grass(40.0f * col , 40.0f * row ); // Prototype design pattern used.
					//std::cout << "Grass: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
					break;
					}
				case 'b':
					{
					m_level[row][col] = new Brick(40.0f * col , 40.0f * row ); // Prototype design pattern used.
					//std::cout << "Brick: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
					break;
					}
					default:break;
				}
				addChild(m_level[row][col]);
				std::cout << "Tile: " << m_level[row][col]->getTransform()->position.x << " " << m_level[row][col]->getTransform()->position.y << std::endl;
				// Construct the Node for a valid tile.
				if (!m_level[row][col]->IsObstacle() && !m_level[row][col]->IsHazard())
				{
					m_level[row][col]->m_node = new PathNode();
					m_level[row][col]->m_node->getTransform()->position.x = m_level[row][col]->getTransform()->position.x + 0.5f * Config::TILE_SIZE;
					m_level[row][col]->m_node->getTransform()->position.y = m_level[row][col]->getTransform()->position.y + 0.5f * Config::TILE_SIZE;
					m_pathNodeVec.push_back(m_level[row][col]->m_node);
					m_pathNodeNum++;
					std::cout << "PathNode[" << row << "][" << col << "] created." << std::endl;
				}
			}
		}
	}
}

void PlayScene::AddConnection()
{
	for (int row = 0; row < Config::ROW_NUM; row++)
	{
		for (int col = 0; col < Config::COL_NUM; col++)
		{
			if (m_level[row][col]->Node() == nullptr) // Now we can test for nullptr.
				continue; // An obstacle or hazard tile has no connections.
			// Make valid connections. Inside map and a valid tile.
			if (row - 1 != -1 && m_level[row - 1][col]->Node() != nullptr) // Tile above. 
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row - 1][col]->Node(),
					Util::distance( m_level[row][col]->Node()->getTransform()->position, m_level[row-1][col]->Node()->getTransform()->position)));
			if (row + 1 != Config::ROW_NUM && m_level[row + 1][col]->Node() != nullptr) // Tile below.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row + 1][col]->Node(),
					Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row + 1][col]->Node()->getTransform()->position)));
			if (col - 1 != -1 && m_level[row][col - 1]->Node() != nullptr) // Tile to Left.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col - 1]->Node(),
					Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row][col - 1]->Node()->getTransform()->position)));
			if (col + 1 != Config::COL_NUM && m_level[row][col + 1]->Node() != nullptr) // Tile to right.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col + 1]->Node(),
					Util::distance(m_level[row][col]->Node()->getTransform()->position, m_level[row][col + 1]->Node()->getTransform()->position)));
		}
	}
}

void PlayScene::setGridLOS()
{
	for(auto m_pTileRow : m_level)
	{
		for (auto m_ptile : m_pTileRow)
		{
			for(auto m_pObstacleRow:m_level)
			{
				for(auto m_pObstacle: m_pObstacleRow)
				{
					if(m_ptile->Node()!=nullptr)
					{
						m_ptile->Node()->setLOS(CollisionManager::LOSCheck(m_ptile->Node(), m_pPlayer, m_pObstacle));
					}					
				}
			}	
		}					
	}
}

void PlayScene::QueueLine(const glm::vec2 start, const glm::vec2 end, const glm::vec4 col)
{
	m_pointVec.push_back(start);
	m_pointVec.push_back(end);
	m_colorVec.push_back(col);
}

void PlayScene::DrawLine()
{
	for (unsigned i = 0, j = 0; i < m_pointVec.size(); i += 2, j++)
	{
		QueueLine(m_pointVec[i], m_pointVec[i + 1], m_colorVec[j]);
	}
	m_pointVec.clear();
	m_colorVec.clear();
	m_pointVec.shrink_to_fit();
	m_colorVec.shrink_to_fit();
}

void PlayScene::DrawPath()
{
	for (unsigned i = 0; i < PathManager::getPath().size(); i++)
	{
		QueueLine({ PathManager::getPath()[i]->GetFromNode()->getTransform()->position.x + 16, PathManager::getPath()[i]->GetFromNode()->getTransform()->position.y + 16 },
			{ Util::HalfwayPoint(PathManager::getPath()[i]->GetFromNode()->Pt(), PathManager::getPath()[i]->GetToNode()->Pt()).x + 16,
			Util::HalfwayPoint(PathManager::getPath()[i]->GetFromNode()->Pt(), PathManager::getPath()[i]->GetToNode()->Pt()).y + 16 }, { 255,255,0,255 });
		QueueLine({ Util::HalfwayPoint(PathManager::getPath()[i]->GetFromNode()->Pt(), PathManager::getPath()[i]->GetToNode()->Pt()).x + 16,
			Util::HalfwayPoint(PathManager::getPath()[i]->GetFromNode()->Pt(), PathManager::getPath()[i]->GetToNode()->Pt()).y + 16 },
			{ PathManager::getPath()[i]->GetToNode()->getTransform()->position.x + 16, PathManager::getPath()[i]->GetToNode()->getTransform()->position.y + 16 }, { 255,128,0,255 });
	}
}




PathNode* PlayScene::getPathNode(int x, int y)
{
	int Index = (x - 1) * Config::ROW_NUM + (y - 1) * Config::COL_NUM;
	return m_pathNodeVec[Index];
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
	m_isDebugMode = false;
	m_pHPressed = false;
	m_pKPressed = false;
	m_pPPressed = false;
	m_pathNodeNum = 0;
	
	//buildGrid();
	LoadMap();
	AddConnection();
	// Plane Sprite
	m_enemyVec.push_back(new Plane(400.0f, 100.0f));
	for (auto m_pEnemy : m_enemyVec)
	{
		addChild(m_pEnemy);
		//std::cout << "Position: " << m_pEnemy->getTransform()->position.x << " " << m_pEnemy->getTransform()->position.y << std::endl;
	}
	//std::cout << "Enemy: " << (int)m_enemyVec.size() << " "<< numberOfChildren()<<std::endl;
	/*m_enemyVec[0]->setStartNode(getPathNode(10, 3));
	m_enemyVec[0]->setEndNode(getPathNode(10, 7));*/

	m_enemyVec[0]->addPathNode(m_pathNodeVec[0]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[1]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[2]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[3]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[4]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[5]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[6]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[7]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[8]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[9]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[10]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[11]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[12]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[13]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[14]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[15]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[16]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[17]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[18]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[19]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[39]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[38]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[37]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[36]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[35]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[34]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[33]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[32]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[31]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[30]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[29]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[28]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[27]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[26]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[25]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[24]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[23]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[22]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[21]);
	m_enemyVec[0]->addPathNode(m_pathNodeVec[20]);
	
	//m_enemyVec[0]->setPath();

	for(auto enemy:m_enemyVec)
	{
		enemy->getTransform()->position =  enemy->getPatrolPath()[0]->getTransform()->position;
		enemy->getRigidBody()->maxSpeed = 10.0f;
	}
	
	

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
}
