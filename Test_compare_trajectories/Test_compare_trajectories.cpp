// Test_compare_trajectories.cpp 
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>

#include "CTrajectory.h"

using namespace std;

int main()
{
    std::cout << "The program reads the reference trajectory from the given file:\n\n";

    ifstream iFile_ReferenceTrajectory("d:\\reference_trajectory.dat");
    
    // checking if we can't open these files for reading their content
    if ( !iFile_ReferenceTrajectory.is_open() )
    {
        // then we output the following message about mistake 
        cerr << "File reference_trajectory.dat could not be opened for reading!" << endl;
        return 1;
    }
  
    // reading data from reference file:
    int iNumOfTrajectories = 0;
    int iNumOfPoints = 0;

    CTrajectory refTrajectory;

    while (!iFile_ReferenceTrajectory.eof())
    {
        iFile_ReferenceTrajectory >> iNumOfTrajectories;
        std::cout << "Number of test trajectories: " << iNumOfTrajectories << endl;

        iFile_ReferenceTrajectory >> iNumOfPoints;
        std::cout << "Number of points in reference trajectory: " << iNumOfPoints << endl;
        
        while (iNumOfPoints--)
        {
            int x, y, t;
            iFile_ReferenceTrajectory >> x >> y >> t;
            refTrajectory.addPoint(t, x, y);
        }

        // TODO: output trajectory with the same t parametr at different points (X,Y), as we must have only one point at the time  
    }
    
    // close file with reference trajectory 
    iFile_ReferenceTrajectory.close();

    refTrajectory.printMap();
    std::cout << endl << endl;

    // Calculate the Length of reference trajectory 
    refTrajectory.calculateLength();

    // Calculate the average speed of reference trajectory 
    refTrajectory.calculateAverageSpeed(refTrajectory.m_mPoints.rbegin()->first - refTrajectory.m_mPoints.begin()->first);

    std::cout << "Now the program reads test trajectories from the given files:\n\n";

    //ifstream iFile_TestTrajectories("d:\\test_trajectory.dat");
    //ifstream iFile_TestTrajectories("d:\\small_trajectory.dat");
    ifstream iFile_TestTrajectories("d:\\large_trajectory.dat");

    if (!iFile_TestTrajectories.is_open())
    {
        // then we output the following message about mistake 
        cerr << "File test_trajectory.dat could not be opened for reading!" << endl;
        return 1;
    }

    // reading data from reference file:
    vector<CTrajectory> vTestTrajectories;
      
    //while (!iFile_TestTrajectories.eof())
    {
        iFile_TestTrajectories >> iNumOfTrajectories;
        std::cout << "Number of test trajectories: " << iNumOfTrajectories << endl;

        int iNumTraj = 0;

        while (iNumOfTrajectories-- && !iFile_TestTrajectories.eof())
        {
            iFile_TestTrajectories >> iNumOfPoints;
            std::cout << "In " << ++iNumTraj << " trajectory " << iNumOfPoints << " points:" << endl;

            CTrajectory testTraj;                       
            while (iNumOfPoints-- && !iFile_TestTrajectories.eof())
            {
                int x, y, t;
                iFile_TestTrajectories >> x >> y >> t;
                testTraj.addPoint(t, x, y);
            }
            testTraj.printMap();

            // Calculate the Length of tested trajectories
            testTraj.calculateLength();

            // Calculate the average speed of tested trajectories
            testTraj.calculateAverageSpeed(testTraj.m_mPoints.rbegin()->first - testTraj.m_mPoints.begin()->first);

            vTestTrajectories.push_back(testTraj);            

            // TODO: output trajectories with the same t parametr at different points (X,Y), as we must have only one point at the time
        }       
    }
    
    // close file with test trajectories 
    iFile_TestTrajectories.close();
    std::cout << endl << endl;

    multimap<double, int> mLengthDifference;
    multimap<double, int> mSpeedDifference;
       
    for (size_t i = 0; i < vTestTrajectories.size(); ++i)
    {
        mLengthDifference.insert( make_pair(abs(refTrajectory.m_dLength - vTestTrajectories[i].m_dLength), i) );
        mSpeedDifference.insert(make_pair(abs(refTrajectory.m_dAverageSpeed - vTestTrajectories[i].m_dAverageSpeed), i));
    }
    
    std::cout << "Choose the metric for comparison data from files, please: 1 - for length / 2 - for speed\n";
    int iMetric = 0;
    std::cin >> iMetric;

    int iCount = 3;   // - number of displaying trajectories (differences between reference and tests trajectories) 
    
    if (iMetric == 1) // work with trajectory length
    {
        std::cout << "Reference traj Length = " << refTrajectory.m_dLength << endl << endl;        

        for (auto it_Len_Dif = mLengthDifference.begin(); it_Len_Dif != mLengthDifference.end() && iCount--; ++it_Len_Dif)
        {
            std::cout << "Length dif of traj # " << it_Len_Dif->second +1 << " = " << it_Len_Dif->first << endl;
            vTestTrajectories[it_Len_Dif->second].printMap();
            std::cout << endl;            
        }
    }
    else  // work with trajectory speed
    {
        std::cout << "Reference traj Avg Speed = " << refTrajectory.m_dAverageSpeed << endl;

        for (auto it_Speed_Dif = mSpeedDifference.begin(); it_Speed_Dif != mSpeedDifference.end() && iCount--; ++it_Speed_Dif)
        {
            std::cout << "Speed dif of traj # " << it_Speed_Dif->second + 1 << " = " << it_Speed_Dif->first  << endl;
            vTestTrajectories[it_Speed_Dif->second].printMap();
            std::cout << endl;            
        }
    }

    return 0;
}


