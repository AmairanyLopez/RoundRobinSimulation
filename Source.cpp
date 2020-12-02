#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//variables
int ID_source = 0;

//Make unique IDs for all the processes.
string ID_producer() {
	ID_source++;
	return ("P" + ID_source);
};


//making a class to hold the process name, process arrival time and process remaining service time
struct process {
	std::string ID;
	int arrival; //Keep track of when it arrived
	float service; //keep track of service
	float burst;
	float trnd = -1; //calculat turn around time
	int end = -1; //time the process completed
	float wait;
};

//vector to hold all processes
vector <process> all_process;
float what_time_is = 0; //keeps track of the time
float context = 0.02; //time slice set to: 
float duration = 0.05;


int main() {
	string line;
	ifstream myfile("times2.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{

			//SEPARATE words for arrival and service time 
			std::string arrive;
			std::string service;
			int first = 0;
			char* words[2];

			size_t space = 0;
			size_t space2;

			space2 = line.find(" ", space);
			arrive = line.substr(space, (space2 - space));
			service = line.substr((space2 - space+2), line.length() + 1);
			
			//cout << arrive << "and" << service << endl;
			
			//store values in notes in a vector
			process Note;
			Note.ID = ID_producer();
			Note.arrival = stoi(arrive);
			Note.service = stoi(service);
			Note.burst = stoi(service);
			all_process.push_back(Note);

		}
		myfile.close();

		//cout << all_process.size() << endl; //Check if all notes were stored
	}

	else cout << "Unable to open file";

	//We will go trough the processes until all of them are done.
	
	while (1) { 
		int all_done = 0;
		//loop through the processes 
		for (int i = 0; i < all_process.size(); i++) {

			//if we reach a process that has not arrived yet, restart from the top
			// We neeed to check a)if the process has arrived, b) if the process finished** 
			//if a process is added at that moment assuming processes are in order of arrival, restart the loop to simulate the queue
			if (all_process[i].arrival == what_time_is && what_time_is != 0) { 
				if (i == all_done) { //check if there are any process running, check for idle time 
					what_time_is += 1; //make time continue to avoid a deadlock
					break;
				}
				else {
					break;
				}
			}
			else if (all_process[i].arrival > what_time_is) {
				if (i == all_done) { //check if there are any process running, check for idle time 
					what_time_is += 1; //make time continue to avoid a deadlock
					break;
				}
				else {
					break;
				}
			}
			else if (all_process[i].end > 0) { //check if processed finished
				//if process finished keep a counter to stop the main loop once all processes have been completed
				all_done++;
				continue;
			}

			//take away from the service time, the duration time, check that the service time is not less than duration
			if (all_process[i].service <= duration) {
				//add service time to total time and then slice time; set service to 0; increase all done counter and set finish time
				what_time_is += all_process[i].service;
				all_process[i].end = what_time_is; //set the finish time to calculate TAT
				all_process[i].trnd = all_process[i].end - all_process[i].arrival; //calculate the turn around time.
				all_process[i].wait = all_process[i].trnd - all_process[i].burst;
				what_time_is += context;
				all_process[i].service = 0;
				all_done++;
			}
			else {
				//substract duration from service; increase the time with the context switch and the process duration; 
				all_process[i].service -= duration;
				what_time_is += context;
				what_time_is += duration;
			}
		
		}


		if (all_done == all_process.size()) {
			break;
		}
	} //Outer loop

	//At this point all of the process have a finish time and therefore a turn around time. 
	//Obtain average
	float sum = 0;
	float wait_sum = 0;
	for (int i = 0; i < all_process.size(); i++) {
		sum += all_process[i].trnd;
		wait_sum += all_process[i].wait;
	}
	// average Turn around time is:
	float ave_trnd = sum / all_process.size();

	//average wait time is:
	float ave_wait = wait_sum/ all_process.size();

	cout << "Average Turn around for time slice: " << duration << " and for context switch: " << context << " is: " << ave_trnd << endl;
	cout << "Average wait time is: " << ave_wait << endl;

	system("pause");
	return 0;
}
