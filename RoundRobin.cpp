#include "RoundRobin.h"

/*
This is a constructor for RoundRobin Scheduler, you should use the extractProcessInfo function first
to load process information to process_info and then sort process by arrival time;

Also initialize time_quantum
*/
RoundRobin::RoundRobin(string file, int time_quantum)
{
	this->extractProcessInfo(file);
	// std::sort(processVec.begin(), processVec.end());

	// for (size_t i = 0; i < processVec.size(); i++) {
	// 	// cerr << processVec.at(i)->get_arrival_time() << endl;
	// }

	this->set_time_quantum(time_quantum);
}

// Schedule tasks based on RoundRobin Rule
// the jobs are put in the order the arrived
// Make sure you print out the information like we put in the document
void RoundRobin::schedule_tasks()
{
	// how many seconds will i take off of remianing time
	// will process complete in this quantum
	// if not complete, readd to queue?
	// print as it goes along
	// burst time could be 0
	// multiuple identifcal timestamps if finish and start at same time
	// no jobs, NOP is pid (-1)


	int cur_time = 0;

	processQ.push(*processVec.at(0));
	processVec.erase(processVec.begin());	// once in the queue, remove it from vector	
	Process cur_proc;

	while (!processVec.empty() || !processQ.empty()) {		// continue until all processes have been completed
		// cerr << "CUR PROC WILL BE " << processQ.front().getPid() << endl;
		cur_proc = processQ.front();

		// cerr << "TIME QUANTUM IS " << this->get_time_quantum() << " AND REMAINING TIME IS " << cur_proc.get_remaining_time() << endl;

		int run = std::min(this->get_time_quantum(), cur_proc.get_remaining_time());
		// cerr << "RUN IS " << run << endl;

		// run it for either the quantum time or however much is left
		int start_time = cur_time;
		for (; cur_time < start_time + run; cur_time++) {
			print(cur_time, cur_proc.getPid(), cur_proc.is_Completed());

			// checks each second for all processes that arrive
			while (!processVec.empty() && cur_time == processVec.front()->get_arrival_time()) {
				processQ.push(*processVec.front());
				processVec.erase(processVec.begin());
			}
			

			cur_proc.Run(1);
			// cerr << "CUR TIME IS " << cur_time << " AND CUR_TIME + RUN IS " << cur_time + run << endl;
		}

		if (!processVec.empty() && cur_time == processVec.front()->get_arrival_time()) {
			processQ.push(*processVec.front());
			processVec.erase(processVec.begin());
		}
 		// update info about the run we just did
		// cur_proc.Run(run);

		// pop if completed, move to back if not
		if (cur_proc.is_Completed()) {
			print(cur_time, cur_proc.getPid(), cur_proc.is_Completed());
			processQ.pop();
		}
		else {		// naturally goes after the new arrivals
			processQ.pop();
			processQ.push(cur_proc);
		}

		if(processQ.empty() && !processVec.empty()) {
			print(cur_time, -1, false);

			while (!processVec.empty() && cur_time == processVec.front()->get_arrival_time()) {
				processQ.push(*processVec.front());
				processVec.erase(processVec.begin());
			}

			cur_time++;
		}

	}

}

/***************************
ALL FUNCTIONS UNDER THIS LINE ARE COMPLETED FOR YOU
You can modify them if you'd like, though :) 
***************************/

// Default constructor
RoundRobin::RoundRobin()
{
	time_quantum = 0;
}

// // testing method
// void RoundRobin::test() {
// 	// conclusions
// 	// erasing first element does move next one to the front
// 	// cerr << "Front of Queue: " << processQ.front().getPid() << endl;

// 	// cerr << "Vector: ";
// 	for (size_t i = 0; i < processVec.size(); i++) {
// 		// cerr << processVec.at(i)->getPid() << " ";
// 	}
// 	// cerr << endl;

// }

// Time quantum setter
void RoundRobin::set_time_quantum(int quantum)
{
	this->time_quantum = quantum;
}

// Time quantum getter
int RoundRobin::get_time_quantum()
{
	return time_quantum;
}

// Print function for outputting system time as part of the schedule tasks function
void RoundRobin::print(int system_time, int pid, bool isComplete)
{
	string s_pid = pid == -1 ? "NOP" : to_string(pid);
	cout << "System Time [" << system_time << "].........Process[PID=" << s_pid << "] ";
	if (isComplete)
		cout << "finished its job!" << endl;
	else
		cout << "is Running" << endl;
}

// Read a process file to extract process information
// All content goes to proces_info vector
void RoundRobin::extractProcessInfo(string file)
{
	// open file
	ifstream processFile(file);
	if (!processFile.is_open())
	{
		perror("could not open file");
		exit(1);
	}

	// read contents and populate process_info vector
	string curr_line, temp_num;
	int curr_pid, curr_arrival_time, curr_burst_time;
	while (getline(processFile, curr_line))
	{
		// use string stream to seperate by comma
		stringstream ss(curr_line);
		getline(ss, temp_num, ',');
		curr_pid = stoi(temp_num);
		getline(ss, temp_num, ',');
		curr_arrival_time = stoi(temp_num);
		getline(ss, temp_num, ',');
		curr_burst_time = stoi(temp_num);
		shared_ptr<Process> p(new Process(curr_pid, curr_arrival_time, curr_burst_time));

		processVec.push_back(p);
	}

	// close file
	processFile.close();
}
