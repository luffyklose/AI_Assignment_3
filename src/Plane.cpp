#include "Plane.h"

#include <algorithm>

#include "TextureManager.h"
#include"Util.h"

Plane::Plane(float x,float y)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));

	// set frame width
	setWidth(40);

	// set frame height
	setHeight(40);

	getTransform()->position = glm::vec2(x,y);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);

	m_detectionRadius = 200;
	m_accel = 0.2;
	m_velMax = 2.0;

	m_buildAnimations();
}

Plane::~Plane()
= default;

void Plane::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	TextureManager::Instance()->playAnimation(
		"spritesheet", getAnimation("plane"),
		x+10.0f, y+10.0f, getWidth(), getHeight(), 0.5f, m_angle, 255, true);
}

void Plane::update()
{
	//if (m_path.empty())
	//{
	//	std::cout << "Path Empty" << std::endl;
	//}
	////std::cout << "Running" << std::endl;
	////std::cout << MAMA::Distance((int)(GetDstP()->x + GetDstP()->w / 2.0f), m_targetNode->x+16, (int)(GetDstP()->y + GetDstP()->h / 2.0f), m_targetNode->y+16) << std::endl;
	//if (Util::distance({ (getTransform()->position.x + getWidth() / 2.0f), m_targetNode->getTransform()->position.x + 20 }, { (getTransform()->position.y + getHeight() / 2.0f), (m_targetNode->getTransform()->position.y + 20) }) <= 10.0)
	//	SetNextNode();
	////std::cout << "Current: " << m_currentNode->x /32<< " " << m_currentNode->y/32 << std::endl;
	////std::cout << "Target: " << m_targetNode->x/32 << " " << m_targetNode->y /32<< std::endl;
	//getDir();
	////std::cout << m_dir << std::endl;
	//MovePlane();
	//
	if(m_isPatrol)
	{
		MovePlanetoPatrolNode();
	}
	
	//getTransform()->position += getRigidBody()->velocity;
	//getRigidBody()->velocity *= getRigidBody()->velocity * 0.9f;
}

void Plane::clean()
{
}

void Plane::m_buildAnimations()
{
	Animation planeAnimation = Animation();

	planeAnimation.name = "plane";
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane1"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane2"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane3"));

	setAnimation(planeAnimation);
}

void Plane::MovePlane()
{
	// Finish up movement.
	// Now use the new slight rotation to generate dx and dy as normal.
	//std::cout <<"before "<< m_vel << std::endl;
	switch (m_dir)
	{
	case left:
	{
		getRigidBody()->velocity.x += m_accel;
		getRigidBody()->velocity.x = std::min(getRigidBody()->velocity.x, m_velMax);
		getRigidBody()->velocity = glm::vec2(-m_vel, 0.0f);
		break;
	}
	case right:
	{
		getRigidBody()->velocity.x += m_accel;
		getRigidBody()->velocity.x = std::min(getRigidBody()->velocity.x, m_velMax);
		getRigidBody()->velocity = glm::vec2(m_vel, 0.0f);
		break;
	}
	case up:
	{
		getRigidBody()->velocity.y += m_accel;
		getRigidBody()->velocity.y = std::min(getRigidBody()->velocity.y, m_velMax);
		getRigidBody()->velocity = glm::vec2(0.0f, -m_vel);
		break;
	}
	case down:
	{
		getRigidBody()->velocity.y += m_accel;
		getRigidBody()->velocity.y = std::min(getRigidBody()->velocity.y, m_velMax);
		getRigidBody()->velocity = glm::vec2(0.0f, m_vel);
		break;
	}
	default:break;
	}
	//std::cout << "after " << m_dX<<" "<<m_dY << std::endl;
}

void Plane::SetNextNode()
{
	//std::cout << "Set Node" << m_nodeIndex<<" "<<(int)s_path.size()<<std::endl;
	if (m_nodeIndex < (int)m_path.size() - 1)
	{
		//std::cout << "Before: Move from (" << m_currentNode->x/32 << "," << m_currentNode->y/32 << ") to (" << m_targetNode->x/32 << "," << m_targetNode->y/32 << ")." << std::endl;
		m_currentNode = m_targetNode;
		m_targetNode = m_path[++m_nodeIndex]->GetToNode();
		//std::cout << "After: Move from (" << m_currentNode->x/32 << "," << m_currentNode->y/32<< ") to (" << m_targetNode->x/32 << "," << m_targetNode->y/32 << ")." << std::endl;
	}
	else
	{
		std::cout << "the last one" << std::endl;
		m_currentNode = m_targetNode;
	}
}

/*void Plane::setPath(std::vector<PathConnection*> path)
{
	m_path = path;
	m_currentNode = m_path[0]->GetFromNode();
	m_targetNode = m_path[0]->GetToNode();
}*/

std::vector<PathConnection*> Plane::getPath()
{
	return m_path;
}


void Plane::getDir()
{
	if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y > m_currentNode-> getTransform()->position.y)
		m_dir = down;
	if (m_targetNode->getTransform()->position.x == m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y < m_currentNode->getTransform()->position.y)
		m_dir = up;
	if (m_targetNode->getTransform()->position.x < m_currentNode->getTransform()->position.x && m_targetNode->getTransform()->position.y == m_currentNode->getTransform()->position.y)
		m_dir = left;
	if (m_targetNode->getTransform()->position.x > m_currentNode->getTransform()->position.x && m_targetNode-> getTransform()->position.y == m_currentNode->getTransform()->position.y)
		m_dir = right;
	if (m_currentNode == m_targetNode)
	{
		PathNode* temp = start_point;
		start_point = end_point;
		end_point = temp;
		temp = nullptr;
		m_nodeIndex = 0;
	}
}