// Test_compare_trajectories.cpp 
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>

#include "CTrajectory.h"


// FUNCTION:	readTrajFromFile 
// DESCRIPTION: function of reading trajectories data from file 
//              and calculating different metrics connecting with trajectories
// INPUT :		filename - file name 
// OUTPUT:		vector<CTrajectory> - vector of readed trajectories
// 
std::vector<CTrajectory>  readTrajFromFile(const std::string& filename)
{
    std::ifstream iFile_Trajectories(filename, std::ios::binary);

    // checking if we can't open file for reading its content
    if (!iFile_Trajectories.is_open())
    {
        throw std::runtime_error("Failed to open file " + filename);       
    }

    uint32_t iNumOfTrajectories = 0;
    std::vector<CTrajectory> vTestTrajectories;
    
    if (!iFile_Trajectories.eof())
    {
        iFile_Trajectories >> iNumOfTrajectories;
        std::cout << "Number of trajectories: " << iNumOfTrajectories << std::endl;

        uint32_t iNumTraj = 0;   
       
        while (iNumOfTrajectories--)
        {
            uint32_t iNumOfPoints = 0;

            iFile_Trajectories >> iNumOfPoints;
            std::cout << "In " << ++iNumTraj << " trajectory " << iNumOfPoints << " points:" << std::endl;

            CTrajectory testTraj;

            while (iNumOfPoints--)
            {
                int32_t x, y, t;
                iFile_Trajectories >> x >> y >> t;
                testTraj.addPoint(t, x, y);
            }
            testTraj.printMap();

            // Calculate the Length of tested trajectories
            testTraj.calculateLength();

            // Calculate the average speed of tested trajectories
            testTraj.calculateAverageSpeed(testTraj.getPoints().rbegin()->first - testTraj.getPoints().begin()->first);

            vTestTrajectories.push_back(testTraj);
        }
    }

    // close file with test trajectories 
    iFile_Trajectories.close();
    std::cout << std::endl << std::endl;

    return vTestTrajectories;
}


// Main function of the programm
//
int main()
{
    try {
        std::cout << "The program reads the reference trajectory from the given file:\n\n";
        auto refTrajectory = readTrajFromFile("d:\\reference_trajectory.dat");

        std::cout << "Now the program reads test trajectories from the given file:\n\n";
        auto vTestTrajectories = readTrajFromFile("d:\\small_trajectory.dat");

        ///////////////////////////////////////////////////////////////////////////////////////////////
        // put the metrics in the appropriate containers sorted in ascending order of the key
        //
        std::multimap<double, int> mLengthDifference;
        std::multimap<double, int> mSpeedDifference;

        for (size_t i = 0; i < vTestTrajectories.size(); ++i)
        {
            mLengthDifference.emplace(std::make_pair(abs(refTrajectory[0].getLength() - vTestTrajectories[i].getLength()), i));
            mSpeedDifference.emplace(std::make_pair(abs(refTrajectory[0].getAverageSpeed() - vTestTrajectories[i].getAverageSpeed()), i));
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////

        std::cout << "Choose the metric for comparison data from files, please: 1 - for length / 2 - for speed\n";
        uint16_t iMetric = 0;
        std::cin >> iMetric;

        uint16_t iCount = 3;   // - number of displaying trajectories nearest to reference traj according chosen metrics 

        if (iMetric == 1) // work with trajectory length
        {
            std::cout << "Reference traj Length = " << refTrajectory[0].getLength() << std::endl << std::endl;

            for (auto it_Len_Dif = mLengthDifference.begin(); it_Len_Dif != mLengthDifference.end() && iCount--; ++it_Len_Dif)
            {
                std::cout << "Length dif of traj # " << it_Len_Dif->second + 1 << " = " << it_Len_Dif->first << std::endl;
                vTestTrajectories[it_Len_Dif->second].printMap();
                std::cout << std::endl;
            }
        }
        else  // work with trajectory speed
        {
            std::cout << "Reference traj Avg Speed = " << refTrajectory[0].getAverageSpeed() << std::endl << std::endl;

            for (auto it_Speed_Dif = mSpeedDifference.begin(); it_Speed_Dif != mSpeedDifference.end() && iCount--; ++it_Speed_Dif)
            {
                std::cout << "Speed dif of traj # " << it_Speed_Dif->second + 1 << " = " << it_Speed_Dif->first << std::endl;
                vTestTrajectories[it_Speed_Dif->second].printMap();
                std::cout << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;   
}


