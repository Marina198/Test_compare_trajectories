// Test_compare_trajectories.cpp 
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>

#include "CTrajectory.h"

int main()
{
    ///////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "The program reads the reference trajectory from the given file:\n\n";

    std::ifstream iFile_ReferenceTrajectory("d:\\reference_trajectory.dat");
    
    // checking if we can't open file for reading its content
    if ( !iFile_ReferenceTrajectory.is_open() )
    {
        // then we output the following message about mistake 
        std::cerr << "File reference_trajectory.dat could not be opened for reading!" << std::endl;
        return 1;
    }
  
    // reading data from reference file:
    int iNumOfTrajectories = 0;
    int iNumOfPoints = 0;
    CTrajectory refTrajectory;

    while (!iFile_ReferenceTrajectory.eof())
    {
        iFile_ReferenceTrajectory >> iNumOfTrajectories;
        std::cout << "Number of test trajectories: " << iNumOfTrajectories << std::endl;

        iFile_ReferenceTrajectory >> iNumOfPoints;
        std::cout << "Number of points in reference trajectory: " << iNumOfPoints << std::endl;
        
        while (iNumOfPoints--)
        {
            int x, y, t;
            iFile_ReferenceTrajectory >> x >> y >> t;
            refTrajectory.addPoint(t, x, y);
        }

        refTrajectory.printMap();
       
        // Calculate the Length of reference trajectory 
        refTrajectory.calculateLength();

        // Calculate the average speed of reference trajectory 
        refTrajectory.calculateAverageSpeed(refTrajectory.getPoints().rbegin()->first - refTrajectory.getPoints().begin()->first);

        // TODO: output trajectory with the same t parametr at different points (X,Y), as we must have only one point at the time  
    }
    
    // close file with reference trajectory 
    iFile_ReferenceTrajectory.close();
    std::cout << std::endl << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////////////

    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "Now the program reads test trajectories from the given files:\n\n";

    //std::ifstream iFile_TestTrajectories("d:\\test_trajectory.dat");
    std::ifstream iFile_TestTrajectories("d:\\small_trajectory.dat");
    //std::ifstream iFile_TestTrajectories("d:\\large_trajectory.dat");

    // checking if we can't open file for reading its content
    if (!iFile_TestTrajectories.is_open())
    {
        // then we output the following message about mistake 
        std::cerr << "File test_trajectory.dat could not be opened for reading!" << std::endl;
        return 1;
    }

    // reading data from test trajectories file:
    std::vector<CTrajectory> vTestTrajectories;
      
    //while (!iFile_TestTrajectories.eof())
    {
        iFile_TestTrajectories >> iNumOfTrajectories;
        std::cout << "Number of test trajectories: " << iNumOfTrajectories << std::endl;

        int iNumTraj = 0;

        while (iNumOfTrajectories-- && !iFile_TestTrajectories.eof())
        {
            iFile_TestTrajectories >> iNumOfPoints;
            std::cout << "In " << ++iNumTraj << " trajectory " << iNumOfPoints << " points:" << std::endl;

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
            testTraj.calculateAverageSpeed(testTraj.getPoints().rbegin()->first - testTraj.getPoints().begin()->first);

            vTestTrajectories.push_back(testTraj);            

            // TODO: output trajectories with the same t parametr at different points (X,Y), as we must have only one point at the time
        }       
    }
    
    // close file with test trajectories 
    iFile_TestTrajectories.close();
    std::cout << std::endl << std::endl;
    ///////////////////////////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////////////////////////
    std::multimap<double, int> mLengthDifference;
    std::multimap<double, int> mSpeedDifference;
       
    for (size_t i = 0; i < vTestTrajectories.size(); ++i)
    {
        mLengthDifference.emplace(std::make_pair(abs(refTrajectory.getLength() - vTestTrajectories[i].getLength()), i));
        mSpeedDifference.emplace(std::make_pair(abs(refTrajectory.getAverageSpeed() - vTestTrajectories[i].getAverageSpeed()), i));
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    

    ////////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "Choose the metric for comparison data from files, please: 1 - for length / 2 - for speed\n";
    int iMetric = 0;
    std::cin >> iMetric;

    int iCount = 3;   // - number of displaying trajectories nearest to reference traj according chosen metrics 
    
    if (iMetric == 1) // work with trajectory length
    {
        std::cout << "Reference traj Length = " << refTrajectory.getLength() << std::endl << std::endl;        

        for (auto it_Len_Dif = mLengthDifference.begin(); it_Len_Dif != mLengthDifference.end() && iCount--; ++it_Len_Dif)
        {
            std::cout << "Length dif of traj # " << it_Len_Dif->second +1 << " = " << it_Len_Dif->first << std::endl;
            vTestTrajectories[it_Len_Dif->second].printMap();
            std::cout << std::endl;            
        }
    }
    else  // work with trajectory speed
    {
        std::cout << "Reference traj Avg Speed = " << refTrajectory.getAverageSpeed() << std::endl << std::endl;

        for (auto it_Speed_Dif = mSpeedDifference.begin(); it_Speed_Dif != mSpeedDifference.end() && iCount--; ++it_Speed_Dif)
        {
            std::cout << "Speed dif of traj # " << it_Speed_Dif->second + 1 << " = " << it_Speed_Dif->first  << std::endl;
            vTestTrajectories[it_Speed_Dif->second].printMap();
            std::cout << std::endl;            
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}


