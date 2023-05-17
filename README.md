To start the program you should input the following command in the PowerShell or Bash from the folder where the executable file Test_compare_trajectories is located:

cat reference_trajectory.dat | ./Test_compare_trajectories test_trajectory.dat length speed time

Where "reference_trajectory.dat" and "test_trajectory.dat" - reference and testing trajectories, 
length speed time - metrics used


The following testing files should also be in this folder:

reference_trajectory.dat
test_trajectory.dat
small_trajectory.dat
large_trajectory.dat

Example:

PS C:\Users\User\source\repos\Test_compare_trajectories\Marina198\Test_compare_trajectories\x64\Debug> cat reference_trajectory.dat | ./Test_compare_trajectories test_trajectory.dat length speed time

Result:

2 closest trajectories by length: 17.12,  to reference trajectory:
Trajectory: 0, length: 17.12, diff: 0.00
Trajectory: 1, length: 10.00, diff: 7.12
2 closest trajectories by speed: 8.56,  to reference trajectory:
Trajectory: 0, speed: 8.56, diff: 0.00
Trajectory: 1, speed: 5.00, diff: 3.56
2 closest trajectories by time: 2.00,  to reference trajectory:
Trajectory: 0, time: 2.00, diff: 0.00
Trajectory: 1, time: 2.00, diff: 0.00 