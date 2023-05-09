// Start preprocessor directive
#ifndef CTrajectory_H
#define CTrajectory_H

#include <map>
#include <iostream>

// CTrajectory class declaration
//
class CTrajectory
{
private:
	// Members
	int m_iNumOfTrajPoints{ 0 };
	std::map<int, std::pair<int, int>> m_mPoints {};
	double m_dLength{ 0.0 };
	double m_dAverageSpeed{ 0.0 };
	
public:
	// Constructor
	CTrajectory();

	// Methods
	void addPoint(int time, int x, int y);
	void calculateLength();
	void calculateAverageSpeed(int iTotalTime);
	void printMap() const;

	// Setters
	void setNumOfTrajPoints(int iNumPoints);
	void setLength(double dLen);
	void setAverageSpeed(double dSpeed);

	// Getters
	int getNumOfTrajPoints() const;
	std::map<int, std::pair<int, int>> getPoints() const;
	double getLength() const;
	double getAverageSpeed() const;	
};

// Stop preprocessor directive
#endif