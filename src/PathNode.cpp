#include "PathNode.h"

PathNode::PathNode()
{
	m_bLOS = false;
	setWidth(40);
	setHeight(40);
	setType(PATH_NODE);
}

PathNode::~PathNode()
{
	
}

void PathNode::update()
{
	
}

void PathNode::draw()
{
	
}

void PathNode::clean()
{
	
}

void PathNode::setLOS(const bool a)
{
	m_bLOS = a;
}

bool PathNode::getLOS() const
{
	return m_bLOS;
}
