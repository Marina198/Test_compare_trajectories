// CTrajectory.cpp - CTrajectory class definition
//

#include "CTrajectory.h"

// Constructor
//
CTrajectory::CTrajectory()
{
	m_iNumOfTrajPoints = 0;
	m_dLength = 0.0;
	m_dAverageSpeed = 0.0;
}

// FUNCTION:	addPoint 
// DESCRIPTION: function of adding pair of points (x,y) to the map, where the key is time
// INPUT :		time - time of the point of trajectory, (x,y) - coordinates of the trajectory
// OUTPUT:		void
// 
void CTrajectory::addPoint(int time, int x, int y)
{
	m_mPoints[time] = make_pair(x,y);
	m_iNumOfTrajPoints++;
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
	pair<int, int> prevPoint = it_Points->second;
	it_Points++;

	for (; it_Points != m_mPoints.end(); it_Points++)
	{
		pair<int, int> currentPoint = it_Points->second;
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
void CTrajectory::printMap()
{
	for (auto it_trajData = m_mPoints.begin(); it_trajData != m_mPoints.end(); it_trajData++)
	{
		cout << it_trajData->first << "  " << it_trajData->second.first << " ; " << it_trajData->second.second << endl;
	}
}