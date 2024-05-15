#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <list>
using namespace std;

void Inputtxt(ifstream &inputFile, string &filenum, bool &check) // Ū�Jinput��txt
{
    string filename;
    cout << "\nInput a file number ([0] Quit): ";
    cin >> filename;
    filenum = filename;
    filename = filename + ".txt";
    inputFile.open(filename);
    if (filenum == "0"){
    	check = false;
	}
    else if (!inputFile) {
        cout << "\n###" << filename << " does not exist! ###" << endl;
        cin.clear();
        check = false;
    } else {
        check = true;
    }
}
string Inttohex(int target){
	if(target <  10){
		return to_string(target);
	}
	else{
		target = target - 9;
		char c = 'A';
		while(target > 1){
			c++;
			target--;
		}
		string temp;
		temp = c;
		return temp;
	}
	
} 
struct Answer {
	string graph;
	vector<int> pid;
	vector<int> waiting;
	vector<int> turnaround;
};
Answer result[6];
// �i�{�����c��
struct Process {
    int pid;        // �i�{ID
    int arrival;    // ��F�ɶ�
    int burst;      // ����ɶ�
	int priority;
	int waiting;
	int turnaround;
	bool done = false;
	Process(){
		
	}
    Process(int id, int bur, int arr, int prior) : pid(id), burst(bur),arrival(arr),priority(prior) {}
};
vector<Process> SortWithId(vector<Process> vec ){
	vector<Process> processes = vec;
    Process temp;
    for(int i = 0; i < processes.size(); i++){
    	for(int j = i+1; j < processes.size(); j++){
    		if(processes[i].pid > processes[j].pid){
    			temp = processes[i];
    			processes[i] = processes[j];
    			processes[j] = temp;
			}
		}
	}
	return processes;
}
// CPU �ի׾���
class Scheduler {
private:
    vector<Process> processes;

public:
    // �s�W�@�Ӷi�{��ի׾�
    void addProcess(int pid, int burst, int arrival, int prior) {
        processes.emplace_back(pid, burst, arrival, prior);
    }
    void push(Process target){
    	processes.push_back(target);
	}
	
	Process findtarget(int pid){
		for(int i =0 ; i < processes.size(); i++){
			if(processes[i].pid == pid){
				return processes[i];
			}
		}
	}
    
    void sort(){
    	Process temp;
    	for(int i = 0; i < processes.size(); i++){
    		for(int j = i+1; j < processes.size(); j++){
    			if(processes[i].arrival > processes[j].arrival){
    				temp = processes[i];
    				processes[i] = processes[j];
    				processes[j] = temp;
				}
    			else if(processes[i].arrival == processes[j].arrival){
    				if(processes[i].pid > processes[j].pid){
     					temp = processes[i];
    					processes[i] = processes[j];
    					processes[j] = temp;   					
					}
				}
			}
		}
	}

    // ���ӥ��A�� (FCFS) �ի׺�k
    void FCFS() {
    	vector<Process> readyQueue = processes;
        int currentTime = 0;
        string graph;
        cout << "FCFS Scheduling:\n";
        for (int i = 0; i < readyQueue.size(); i++) {
            if (currentTime < readyQueue[i].arrival)
                currentTime = readyQueue[i].arrival;  // ���ݨ�i�{��F�ɶ�
            
			string temp = Inttohex(readyQueue[i].pid);
			for(int i = 0; i < readyQueue[i].burst; i++){
				graph = graph + temp;
			}
			readyQueue[i].turnaround = currentTime + readyQueue[i].burst - readyQueue[i].arrival;
			readyQueue[i].waiting = readyQueue[i].turnaround - readyQueue[i].burst;
            //cout << "Process " << readyQueue[i].pid << " waiting time: " << readyQueue[i].waiting
            //     << " and turnaround time: " << readyQueue[i].turnaround << endl;

            currentTime += readyQueue[i].burst;  // ��s��e�ɶ�
        }
        readyQueue = SortWithId(readyQueue);
        for(int i = 0; i < readyQueue.size(); i++){
        	result[0].pid.push_back(readyQueue[i].pid);
        	result[0].waiting.push_back(readyQueue[i].waiting);
        	result[0].turnaround.push_back(readyQueue[i].turnaround);
            //cout << "Process " << readyQueue[i].pid << " waiting time: " << readyQueue[i].waiting
            //     << " and turnaround time: " << readyQueue[i].turnaround << endl;        	
		}
		result[0].graph = graph;
        //cout << "Graph:" << graph << endl;
        cout << result[0].graph << endl;
        for(int i = 0 ; i <result[0].pid.size() ; i++){
        	cout << result[0].pid[i] << "	" << result[0].waiting[i] << "	" << result[0].turnaround[i] << endl;
		}
    }

    // ���� (Round Robin) �ի׺�k
    void RR(int quantum) {
        queue<Process> readyQueue;
        vector<Process> output;
        int currentTime = 0;
        int idx = 0;
        bool done = true;
        Process temp;
        string graph;
        cout << "\nRound Robin Scheduling with Quantum " << quantum << ":\n";

        while (!readyQueue.empty() || idx < processes.size()) {
            // �N��F�ɶ��p�󵥩�ثe�ɶ����i�{�[�J�N�����C
            while (idx < processes.size() && processes[idx].arrival <= currentTime) {
                readyQueue.push(processes[idx]);
                idx++;
            }
        	if(done == false){
        		readyQueue.push(temp);
        		done = true;
			}
            if (readyQueue.empty()) {
                currentTime = processes[idx].arrival;
                continue;
            }

            Process currentProc = readyQueue.front();
            string id = Inttohex(currentProc.pid);
            readyQueue.pop();

            //cout << "Process " << currentProc.pid << " starts at " << currentTime;

            if (currentProc.burst <= quantum) {
                // �i�{���槹��
                for(int i = 0; i < currentProc.burst; i++){
                	graph = graph + id;
				}
				Process origin = findtarget(currentProc.pid);
                currentTime += currentProc.burst;
				currentProc.turnaround = currentTime- currentProc.arrival;
				currentProc.waiting = currentProc.turnaround - origin.burst;                
                //cout << " and ends at " << currentTime << endl;
				output.push_back(currentProc);                
                done = true;
            } else {
                // �i�{����@�Ӷq�l���^�N�����C
                for(int i =0; i < quantum; i++){
                	graph = graph + id;
				}
                currentTime += quantum;
                //cout << " and is preempted at " << currentTime << endl;
                currentProc.burst -= quantum;
                temp = currentProc;
                done = false;
            }
        }
        output = SortWithId(output);
        for(int i = 0; i < output.size(); i++){
        	result[1].pid.push_back(output[i].pid);
        	result[1].waiting.push_back(output[i].waiting);
        	result[1].turnaround.push_back(output[i].turnaround);
            //cout << "Process " << readyQueue[i].pid << " waiting time: " << readyQueue[i].waiting
            //     << " and turnaround time: " << readyQueue[i].turnaround << endl;        	
		}
		result[1].graph = graph;
        //cout << "Graph:" << graph << endl;
        cout << result[1].graph << endl;
        for(int i = 0 ; i <result[1].pid.size() ; i++){
        	cout << result[1].pid[i] << "	" << result[1].waiting[i] << "	" << result[1].turnaround[i] << endl;
		}
        //cout << "Graph:" << graph << endl;
    }
    void SJF() {
    	vector<Process> output;
    	vector<Process> remainingJobs = processes;
    	int currentTime = 0;
    	string graph;
    	cout << "\nShortest Job First Schduling " <<  ":\n";
    	while (!remainingJobs.empty()) {
    		vector<Process> readyQueue;
    		int i;
            for (i = 0; i < remainingJobs.size() && remainingJobs[i].arrival <= currentTime; i++) {
                readyQueue.push_back(remainingJobs[i]);
            }
            if (readyQueue.empty()) {
                currentTime = remainingJobs.front().arrival;
                continue;
            }
            int min = INT_MAX;
            int save;
			for(int a = 0; a < readyQueue.size(); a++){
				if(min > readyQueue[a].burst){
					min = readyQueue[a].burst;
					save = a;
				}
			}	
     		Process& job = remainingJobs[save];
			string temps = Inttohex(job.pid);
			for(int i = 0; i < job.burst; i++){
				graph = graph + temps;
			}
        	currentTime += job.burst;
			job.turnaround = currentTime- job.arrival;
			job.waiting = job.turnaround - job.burst;
			output.push_back(job);
			//cout << job.pid << " waiting: " << job.waiting << " turnaround: " << job.turnaround << endl;
        	vector<Process> temp;
        	for(int j = 0; j < remainingJobs.size(); j++){
        		if(job.pid != remainingJobs[j].pid){
        			temp.push_back(remainingJobs[j]);
				}
			}
        	remainingJobs = temp;
    	}
        output = SortWithId(output);
        for(int i = 0; i < output.size(); i++){
        	result[2].pid.push_back(output[i].pid);
        	result[2].waiting.push_back(output[i].waiting);
        	result[2].turnaround.push_back(output[i].turnaround);
            //cout << "Process " << readyQueue[i].pid << " waiting time: " << readyQueue[i].waiting
            //     << " and turnaround time: " << readyQueue[i].turnaround << endl;        	
		}
		result[2].graph = graph;
        //cout << "Graph:" << graph << endl;
        cout << result[2].graph << endl;
        for(int i = 0 ; i <result[2].pid.size() ; i++){
        	cout << result[2].pid[i] << "	" << result[2].waiting[i] << "	" << result[2].turnaround[i] << endl;
		}

    }
    void SRTF() {
    	cout << "\nSRTF Schduling " <<  ":\n";
    	vector<Process> output;
    	vector<Process> remainingJobs = processes;
    	int currentTime = 0;
    	string graph;
    	int total = 0;
		for(int i = 0; i < remainingJobs.size(); i++){
			total = total + remainingJobs[i].burst;
		}
    	for (int currentTime = 0; currentTime <= total ; currentTime++) {
    		vector<Process> readyQueue;
            for (int i = 0; i < remainingJobs.size() && remainingJobs[i].arrival <= currentTime; i++) {
                readyQueue.push_back(remainingJobs[i]);
            }
            if(readyQueue.size() == 0){

            	continue;
			}
            int min = INT_MAX;
            int save;
			for(int a = 0; a <readyQueue.size(); a++){
				if(min > readyQueue[a].burst && readyQueue[a].burst>0){
					min = readyQueue[a].burst;
					save = a;
				}
			}
			string temps = Inttohex(readyQueue[save].pid);
			graph = graph + temps;			
			readyQueue[save].burst--;
			if(readyQueue[save].burst==0){
				Process origin = findtarget(readyQueue[save].pid);
				readyQueue[save].turnaround = (currentTime+1)- readyQueue[save].arrival;
				readyQueue[save].waiting = readyQueue[save].turnaround - origin.burst;
				output.push_back(readyQueue[save]);				
			}
			for(int i = 0; i < remainingJobs.size(); i++){
				if(readyQueue[save].pid == remainingJobs[i].pid){
					remainingJobs[i].burst--;
				}
			}

		}
    	
        output = SortWithId(output);
        for(int i = 0; i < output.size(); i++){
        	result[3].pid.push_back(output[i].pid);
        	result[3].waiting.push_back(output[i].waiting);
        	result[3].turnaround.push_back(output[i].turnaround);
            //cout << "Process " << readyQueue[i].pid << " waiting time: " << readyQueue[i].waiting
            //     << " and turnaround time: " << readyQueue[i].turnaround << endl;        	
		}
		result[3].graph = graph;
        //cout << "Graph:" << graph << endl;
        cout << result[3].graph << endl;
        for(int i = 0 ; i <result[3].pid.size() ; i++){
        	cout << result[3].pid[i] << "	" << result[3].waiting[i] << "	" << result[3].turnaround[i] << endl;
		}

    }
};

int main() {
    Scheduler scheduler;
    string filename;
    ifstream inputFile;
    int order;
    int timeslice;
    bool check = false;
    while((inputFile.eof() || check == false)&&filename!="0"){
        Inputtxt(inputFile, filename, check);
	}
    inputFile >> order >> timeslice;
    inputFile.ignore( INT16_MAX, '\n' );
    inputFile.ignore( INT16_MAX, '\n' );
    // �p�Gtxt���S�F�� �N����
    if (inputFile.eof() || check == false ) {
        cout << "\n### Get nothing from the file input" << filename << ".txt ! ###\n";
        cout << "\nThere is no data!\n";
    }
    else{
    	Process temp;
		while ( inputFile >> temp.pid >> temp.burst >> temp.arrival >> temp.priority ) {
			scheduler.push(temp);
		}
		inputFile.close();
	}

    // �K�[�i�{ (pid, arrival time, burst time)
    
    /*scheduler.addProcess(5,6,26,13);
    scheduler.addProcess(13,1,7,2);
    scheduler.addProcess(6,5,1,7);
    scheduler.addProcess(27,6,3,7);
    scheduler.addProcess(2,3,30,13);
	scheduler.addProcess(1,2,13,5);
	scheduler.addProcess(9,4,1,6);
	scheduler.addProcess(10,8,2,13);
	scheduler.addProcess(0,4,36,1);
	scheduler.addProcess(8,2,23,12);
	scheduler.addProcess(7,1,3,16);
	scheduler.addProcess(29,6,20,8);
	scheduler.addProcess(4,3,18,10);
	scheduler.addProcess(20,3,15,14);
	scheduler.addProcess(3,4,22,3);*/ 
	/*scheduler.addProcess(1,8,0,2);
	scheduler.addProcess(2,4,2,2);
	scheduler.addProcess(3,9,5,2);
	scheduler.addProcess(4,5,8,2);*/
	scheduler.sort();	
    scheduler.FCFS();
    scheduler.RR(3);
	scheduler.SJF();
	scheduler.SRTF();
	
    return 0;
}


