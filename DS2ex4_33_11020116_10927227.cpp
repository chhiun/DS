// 電資三 11020116 潘皓群  10927227 姜智紳 
// 有使用C++11

#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <chrono>
using namespace std;

void GetCommand(int &command)
{
    while (true) {
        std::cout << "\n";
        std::cout << "**** Graph data manipulation *****\n";
        std::cout << "* 0. QUIT                        *\n";
        std::cout << "* 1. Build adjacency lists       *\n";
        std::cout << "* 2. Compute connection counts   *\n";
        std::cout << "**********************************\n";
        std::cout << "Input a choice(0, 1, 2): ";
        std::cin >> command;
        // If the command is valid, exit the loop
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nCommand does not exist!\n";
            continue;
        }
        if (command >= 0 && command <= 2) {
            break;
        } else {
            std::cout << "\nCommand does not exist!" << "\n";
        }
    }
}

void Checkinputbin(ifstream &inputFile, string &filenum, bool &check) // 讀入input的txt
{
    cout << "\nInput a file number ([0] Quit): ";
    cin >> filenum;
    string filename;
    filename = "pairs" + filenum + ".bin";
    inputFile.open(filename);
    if (filenum == "0"){
    	check = false;
	}
    if (!inputFile) {
        cout << "\n### " << filename << " does not exist! ###" << endl;
        cin.clear();
        check = false;
    } else {
        check = true;
    }
    inputFile.close();
}

int partition(std::vector<std::string>& arr, int low, int high) {
    std::string pivot = arr[high]; // 選擇最右邊的元素作為樞紐
    int i = low - 1; // i是較小元素的索引

    for (int j = low; j <= high - 1; j++) {
        // 如果當前元素小於或等於樞紐
        if (arr[j] <= pivot) {
            i++; // 增加較小元素的索引
            std::swap(arr[i], arr[j]); // 交換
        }
    }
    std::swap(arr[i + 1], arr[high]); // 交換樞紐元素到正確的位置
    return (i + 1);
}

// 快速排序函數
void quickSort(std::vector<std::string>& arr, int low, int high) {
    if (low < high) {
        // pi是分區索引，arr[pi]已經排序好
        int pi = partition(arr, low, high);

        // 分別排序pi索引左右兩邊的元素
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
struct Connection{
	string receiveID;
	float weight;
};
int Conpartition(vector<Connection>& arr, int low, int high) {
    string pivot = arr[high].receiveID; // 選擇最右邊的元素作為樞紐
    int i = low - 1; // i是較小元素的索引

    for (int j = low; j <= high - 1; j++) {
        // 如果當前元素小於或等於樞紐
        if (arr[j].receiveID <= pivot) {
            i++; // 增加較小元素的索引
            std::swap(arr[i], arr[j]); // 交換
        }
    }
    std::swap(arr[i + 1], arr[high]); // 交換樞紐元素到正確的位置
    return (i + 1);
}

// 快速排序函數
void ConquickSort(vector<Connection>& arr, int low, int high) {
    if (low < high) {
        // pi是分區索引，arr[pi]已經排序好
        int pi = Conpartition(arr, low, high);

        // 分別排序pi索引左右兩邊的元素
        ConquickSort(arr, low, pi - 1);
        ConquickSort(arr, pi + 1, high);
    }
}
struct Sortsupport{
	string id;
	int count;
};
bool compare(const Sortsupport& a, const Sortsupport& b) {
    if (a.count != b.count) {
        return a.count > b.count; // count大的在前
    }
    return a.id < b.id; // count相同，id小的在前
}

// 分區函數，將數組分成兩部分，左邊是比樞紐小的，右邊是比樞紐大的
int Suppartition(std::vector<Sortsupport>& arr, int low, int high) {
    Sortsupport pivot = arr[high]; // 選擇最右邊的元素作為樞紐
    int i = low - 1; // i是較小元素的索引

    for (int j = low; j <= high - 1; j++) {
        // 使用比較函數判斷是否需要交換
        if (compare(arr[j], pivot)) {
            i++; // 增加較小元素的索引
            std::swap(arr[i], arr[j]); // 交換
        }
    }
    std::swap(arr[i + 1], arr[high]); // 交換樞紐元素到正確的位置
    return (i + 1);
}

// 快速排序函數
void SupquickSort(std::vector<Sortsupport>& arr, int low, int high) {
    if (low < high) {
        // pi是分區索引，arr[pi]已經排序好
        int pi = Suppartition(arr, low, high);

        // 分別排序pi索引左右兩邊的元素
        SupquickSort(arr, low, pi - 1);
        SupquickSort(arr, pi + 1, high);
    }
}
class AdjList{
	private:
		vector<string> alldataID;
		unordered_map<string, vector<Connection>> adjList;
		int IDcount = 0;
	public:
		bool isEmpty(){
			if (alldataID.size()==0)
				return true;
			else
				return false;
		}
		void clear(){
			alldataID.clear();
			adjList.clear();
		}
		void readfile(string filename){
			filename = "pairs"+filename+".bin";
    		ifstream inputFile(filename, ios::binary);
    		while (!inputFile.eof()) {
    		    char sendID[12];
    		    char receiveID[12];
    		    float weight;
    		    inputFile.read(sendID, 12);
    		    if (inputFile.eof()){
    		    	break;
				}
    		    inputFile.read(receiveID, 12);
    		    inputFile.read(reinterpret_cast<char*>(&weight), sizeof(float));
    		    bool send = false;
    		    bool receive = false;
				string sendIDStr = sendID;
				string recieveIDStr = receiveID;
				//若ID已經存在alldata中則不再存入 
				for(int i = 0; i < alldataID.size(); i++){
					if(alldataID[i]==sendIDStr){
						send = true;
						break;
					}
				}
				for(int i = 0; i < alldataID.size(); i++){
					if(alldataID[i]==recieveIDStr){
						receive = true;
						break;
					}
				}
				if(send == false){
					alldataID.push_back(sendIDStr);	
				}
				if(receive == false){
					alldataID.push_back(recieveIDStr);
				}
 				
				//將這次的message存入sender的adjlist中 
				Connection connect = {recieveIDStr, weight};
    		    adjList[sendIDStr].push_back(connect);
    		    //cout << sendIDStr << " " << receiveID << " " << weight << endl; 
    		}
			
			quickSort(alldataID,0,alldataID.size()-1);
    		for (auto &data : adjList) {
				ConquickSort(data.second,0,data.second.size()-1);
    		}
			inputFile.close();		
		}		
		void SaveAdj(string filename){
			filename = "pairs"+filename+".adj";
    		ofstream outputFile(filename, ios::binary);
    		if(!outputFile){
    			cout << "Error opening file: " << filename << endl;
    			return;
			}
    		int idcount = alldataID.size();

			cout << "\n<<< There are " << idcount << " IDs in total. >>>\n"; 
			outputFile << "<<< There are " << idcount << " IDs in total. >>>\n"; 
			
			int nodecount = 0;
    		for (int i = 0; i < alldataID.size(); i++) {
        		outputFile << "[" << std::setw(3) << i+1 << "] " << alldataID[i] << ": \n";

        		auto target = adjList.find(alldataID[i]);
        		if (target != adjList.end()) {
            		auto &connect = target->second;
            		
            		for (int i = 0; i < connect.size(); ++i) {
                		if(i%12 == 0 && i > 0){
                			outputFile << "\n";
						}
                		outputFile << "\t(" << std::setw(2) << (i + 1) << ") " << connect[i].receiveID << ",   " << std::fixed 
						<< std::setprecision(2) << connect[i].weight;            		            
                		nodecount++;
            		}
        		}
        		outputFile << "\n";
    		}

			cout << "\n<<< There are " << nodecount << " nodes in total. >>>\n";
			outputFile << "<<< There are " << nodecount << " nodes in total. >>>\n";	
			IDcount = idcount;	  
			outputFile.close();
		}
		void Connectioncount(string filename){
			filename = "pairs"+filename+".cnt";
    		ofstream outputFile(filename, ios::binary);
    		if(!outputFile){
    			cout << "Error opening file: " << filename << endl;
    			return;
			}
			else{
				unordered_map<string, vector<Connection>> tempList = adjList;
				unordered_map<std::string, std::vector<std::string>> visitedNodes;
				unordered_map<std::string, int> connectionCounts;
				
				for(auto &data : adjList){
					string start = data.first;
					vector<string> visited;
					queue<string> q;
					visited.push_back(start);
					q.push(start);
					//cout << data.first << " " << endl;
					//利用queue來實現BFS 
        			while (!q.empty()) {
            			string currentID = q.front();
            			//cout << "Now " << currentID << ":" << endl ; 
            			q.pop();
            			if (tempList.count(currentID)) {
            				//如果當前ID有連接的receiveID 
                			for (const auto &data : tempList[currentID]) {
                				//就去adjlist裡面取他的資料 
                				//cout << data.receiveID << " " << data.weight << endl;
                				bool found = false;
                				//將visited裡面沒有且他有相連的receiveID放入visited，並且放入queue中 
                				for(int i = 0; i < visited.size(); i++){
                					if(visited[i] == data.receiveID){
                						found = true;
                						break;
									}
								}
                			    if (found == false) {
                			        visited.push_back(data.receiveID);
                			        q.push(data.receiveID);
                			    }
                			}
            			}
        			}
        			//把自己移除掉，自己不算在連通元裡面 
					vector<string> temp;
                	for(int i = 0; i < visited.size(); i++){
                		if(visited[i] != start){
                			temp.push_back(visited[i]);
						}	
					}
					visited = temp;
        			temp.clear();
        			//把visited先排序完 
        			quickSort(visited,0,visited.size()-1);				
					//記錄當前sender的連通數，並儲存通過的receiveID 
        			connectionCounts[start] = visited.size();
        			visitedNodes[start] = visited;																
				}
				//利用Sortsupport把connectionCounts排序 
				vector<Sortsupport> sorted;
				for(auto & data :connectionCounts ){
					Sortsupport sup;
					sup.id = data.first;
					sup.count = data.second;
					sorted.push_back(sup);
				}
				SupquickSort(sorted,0,sorted.size()-1);


				//開始output檔案 
    			outputFile << "<<< There are " << IDcount << " IDs in total. >>>\n";
				cout << "\n<<< There are " << IDcount << " IDs in total. >>>\n";

    			for (int i = 0; i < sorted.size(); i++) {

        			outputFile << "[" << std::setw(3) << i+1 << "] " << sorted[i].id << "(" << sorted[i].count << "): \n" ;

        			auto &nodes = visitedNodes[sorted[i].id];
        			
            		for (int i = 0; i < nodes.size(); i++) {
                		if(i%12 == 0 && i > 0){
                			outputFile << "\n";
						}
						outputFile << "\t(" << std::setw(2) << (i + 1) << ") " << nodes[i];
            		}        			
        			outputFile << "\n";
    			}			
			}
					  
			outputFile.close();
		}
};


int main()
{
    int command = 0;
    GetCommand(command);
    ifstream inputFile;
    ofstream outputFile;
    string filenum;
    bool check = false;
    AdjList list;
    // 當指令不為0，就繼續讀取指令
    while (command != 0) {
        if (command == 1) {
        	list.clear();
            Checkinputbin(inputFile, filenum, check);
            // 如果txt內沒東西 就結束
            if (inputFile.eof() || check == false ) {
                //cout << "\n### Get nothing from the file input" << filenum << ".txt ! ###\n";
                //cout << "\nThere is no data!\n";
            } 
			else {
				auto start = std::chrono::high_resolution_clock::now();
            	list.readfile(filenum);
            	list.SaveAdj(filenum);
				auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                std::cout << "\nElapsed time:  " << elapsed.count() << " ms.\n";            
            }
            
        }
        else if(command == 2){
        	if(list.isEmpty()){
        		cout << "\n### There is no graph and choose 1 first. ###\n";
			}
			else{
				auto start = std::chrono::high_resolution_clock::now();
				list.Connectioncount(filenum);
				auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                std::cout << "\nElapsed time:  " << elapsed.count() << " ms.\n";				
			}
        	
		}
		else {
            printf("Command does not exist!\n"); // 錯誤指令
        }
        GetCommand(command);
    }

    return 0;
}
