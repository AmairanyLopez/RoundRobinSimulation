# RoundRobinSimulation
Program that simulates the Round Robin scheduling technique.

The Round Robin simulation reads a file that contains arrival times and service times for multiple processes. These times are stored in a structure called a process that contains 
arrival time, service time, finish time, turn around time and wait time.
The process are inserted into a vector. The vector goes through a nested loop. The outer loop does not stop until all the process are completed and the inner loop
restarts every time an inactive process (process has not arrived based on current time) this is to simulate the active processes queue vs the process that are waiting for the
long term scheduler to be added. This only works once the processes are arranged by arrival time.

The idle time, when the resources are waiting for a new process to arrive are handled by adding 1 second per iteration without any active processes until a new process is "added
to the active queue".

The code contains comments to explain everything.
