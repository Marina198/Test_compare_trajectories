// Start preprocessor directive
#ifndef CTrajectory_H
#define CTrajectory_H

#include <map>
#include <iostream>

using namespace std;

// CTrajectory class declaration
//
class CTrajectory
{
private:
	
public:
	// Constructor
	CTrajectory();

	// Methods
	void addPoint(int time, int x, int y);
	void calculateLength();
	void calculateAverageSpeed(int iTotalTime);
	void printMap();
	 
	// Members
	int m_iNumOfTrajPoints;
	map<int, pair<int, int>> m_mPoints;
	double m_dLength;
	double m_dAverageSpeed;
};

// Stop preprocessor directive
#endif