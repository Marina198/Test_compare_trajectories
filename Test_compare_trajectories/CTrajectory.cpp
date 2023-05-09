// CTrajectory.cpp - CTrajectory class definition
//

#include "CTrajectory.h"

// Constructor
//
CTrajectory::CTrajectory()
{
}

// FUNCTION:	setNumOfTrajPoints 
// DESCRIPTION: setter function for m_iNumOfTrajPoints
// INPUT :		iNumPoints - number of tragectory points
// OUTPUT:		void
// 
void CTrajectory::setNumOfTrajPoints(int iNumPoints)
{
	m_iNumOfTrajPoints = iNumPoints;
}

// FUNCTION:	getNumOfTrajPoints 
// DESCRIPTION: getter function for m_iNumOfTrajPoints
// INPUT :		void
// OUTPUT:		int
// 
int CTrajectory::getNumOfTrajPoints() const
{
	return m_iNumOfTrajPoints;
}

// FUNCTION:	addPoint 
// DESCRIPTION: function of adding pair of points (x,y) to the map, where the key is time
// INPUT :		time - time of the point of trajectory, (x,y) - coordinates of the trajectory
// OUTPUT:		void
// 
void CTrajectory::addPoint(int time, int x, int y)
{
	m_mPoints.emplace(time, std::make_pair(x, y));
	m_iNumOfTrajPoints++;
}

// FUNCTION:	getPoints 
// DESCRIPTION: getter function for m_mPoints
// INPUT :		void
// OUTPUT:		std::map<int, std::pair<int, int>>
// 
std::map<int, std::pair<int, int>> CTrajectory::getPoints() const
{
	return m_mPoints;
}

// FUNCTION:	setLength 
// DESCRIPTION: setter function for m_dLength
// INPUT :		dLen - the length of the tragectory 
// OUTPUT:		void
// 
void CTrajectory::setLength(double dLen)
{
	m_dLength = dLen;
}

// FUNCTION:	getLength 
// DESCRIPTION: getter function for m_dLength
// INPUT :		void
// OUTPUT:		double
// 
double CTrajectory::getLength() const
{
	return m_dLength;
}

// FUNCTION:	setAverageSpeed 
// DESCRIPTION: setter function for m_dAverageSpeed
// INPUT :		dSpeed - the speed of the tragectory 
// OUTPUT:		void
// 
void CTrajectory::setAverageSpeed(double dSpeed)
{
	m_dAverageSpeed = dSpeed;
}

// FUNCTION:	getAverageSpeed 
// DESCRIPTION: getter function for m_dAverageSpeed
// INPUT :		void
// OUTPUT:		double
// 
double CTrajectory::getAverageSpeed() const
{
	return m_dAverageSpeed;
}

// FUNCTION:	calculateLength 
// DESCRIPTION: function of calculating the Length (total distance) of the whole trajectory
// INPUT :		void
// OUTPUT:		void
// 
void CTrajectory::calculateLength()
{
	m_dLength = 0.0;
	auto it_Points = m_mPoints.begin();
	std::pair<int, int> prevPoint = it_Points->second;
	it_Points++;

	for (; it_Points != m_mPoints.end(); it_Points++)
	{
		std::pair<int, int> currentPoint = it_Points->second;
		m_dLength += sqrt(pow(currentPoint.first - prevPoint.first, 2) + pow(currentPoint.second - prevPoint.second, 2));
		prevPoint = currentPoint;
	}
}

// FUNCTION:	calculateAverageSpeed 
// DESCRIPTION: function of calculating average speed of the whole trajectory
// INPUT :		iTotalTime - total time of the whole trajectory
// OUTPUT:		void
// 
void CTrajectory::calculateAverageSpeed(int iTotalTime)
{
	if (iTotalTime == 0)
	{
		m_dAverageSpeed = 0.0;
		return;
	}

	double dTotalDistance = m_dLength;
	m_dAverageSpeed = dTotalDistance / iTotalTime;
}

// FUNCTION:	printMap 
// DESCRIPTION: function of displaying information on the screen: time and pair of points (x,y) at this time
// INPUT :		void
// OUTPUT:		void
// 
void CTrajectory::printMap() const
{
	for (const auto& elem : m_mPoints)
	{
		std::cout << elem.first << "  " << elem.second.first << " ; " << elem.second.second << std::endl;
	}
}