#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


//making a class to hold the process name, process arrival time and process remaining service time
struct process {
	float arrival; //Keep track of when it arrived
	float service; //keep track of service
	float burst;
	float trnd = -1; //calculat turn around time
	float end = -1; //time the process completed
	float wait;
};

//vector to hold all processes
vector <process> all_process;
vector <process> active;
float what_time_is = 0; //keeps track of the time
float context = 0; //time slice set to: 
float duration = 2;
float sum = 0.0;
float wait_sum = 0.0;
float sizetotal = 0.0;

void checking();


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
			float first = 0;
			char* words[2];

			size_t space = 0;
			size_t space2;

			space2 = line.find(" ", space);
			arrive = line.substr(space, (space2 - space));
			service = line.substr((space2 - space+2), line.length() + 1);
			
			//cout << arrive << "and" << service << endl;
			
			//store values in notes in a vector
			process Note;
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
	sizetotal = all_process.size();
	
	
	while (1) { 
		
		bool noextra = 0;
		checking();

		int finished = 0;
		//go through current processes and calculate turn around and wait time
		if (!active.empty()) {
			for (int i = 0; i < active.size(); i++) {
				//if less than duration
				if (active[i].service <= duration) {
					what_time_is += active[i].service;
					active[i].end = what_time_is; //set the finish time to calculate TAT
					active[i].trnd= active[i].end - active[i].arrival;
					sum += active[i].trnd; //calculate the turn around time.
					wait_sum += active[i].trnd - active[i].burst;
					what_time_is += context;
					active[i].service = 0;
					noextra = 1;
					finished++;
					checking(); //on every time increment, check if process arrived
				}
				else {
					active[i].service -= duration;
					what_time_is += context;
					what_time_is += duration;
					checking();
				}
			}
		}

		int matched = 0;
		//delete completed processes.
		while (1) {
			if (!active.empty()) {
				for (int i = 0; i < active.size(); i++) {
					if (active[i].service == 0.0) {
						active.erase(active.begin() + i);
						matched++;
						break;
					}
				}
			}
			if (matched == finished) {
				break;
			}
		}


		//if active is empty increase time by duration
		if (active.empty() && noextra == 0) {
			what_time_is += duration;
		}


		//stop loop when both vectors are empty
		if (all_process.empty() && active.empty()) {
			break;
		}
	} //Outer loop

	//At this point all of the process have a finish time and therefore a turn around time. 
	//Obtain average
	
	// average Turn around time is:
	float ave_trnd = sum / sizetotal;

	//average wait time is:
	float ave_wait = wait_sum/ sizetotal;

	cout << "Average Turn around for time slice: " << duration << " and for context switch: " << context << " is: " << ave_trnd << endl;
	cout << "Average wait time is: " << ave_wait << endl;

	system("pause");
	return 0;
}

void checking() {
	int deleted = 0;
	//now that we have the vector with processes check if any of all process can be added to active
	if (!all_process.empty()) {
		for (int i = 0; i < all_process.size(); i++) {
			if (all_process[i].arrival <= what_time_is) {
				//add to active queue
				active.push_back(all_process[i]);
				deleted++;
			}
			else {
				break;
			}
		}
	}

	int yadeleted = 0;
	while (1) {
		//now delete active from all
		if (!all_process.empty()) {
			for (int i = 0; i < all_process.size(); i++) {
				if (all_process[i].arrival <= what_time_is) {
					all_process.erase(all_process.begin() + i);
					yadeleted++;
					break;
				}
			}
		}
		if (yadeleted == deleted) {
			break;
		}
	}

};
