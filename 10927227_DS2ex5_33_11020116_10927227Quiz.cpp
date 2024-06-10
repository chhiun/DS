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
#include <stack>
#include <random>
using namespace std;
int maximum = 300;
void GetCommand(string &command)
{
    while (true) {
        std::cout << "\n";
        std::cout << "*** The buffer size is " << maximum <<"\n***********************************************\n";
        std::cout << "Input a number in [1, 60000]: ";
        cin >> maximum;
        cout << "\n\n***********************************************\n";
        std::cout << " On-machine Exercise 05                       *\n";
        std::cout << " Mission 1: External Merge Sort on a Big File *\n";
        std::cout << " Mission 2: Construction of Primary Index     *\n";
        std::cout << " Mission 3: Range Search by Primary Index     *\n";
        std::cout << "***********************************************\n";
        std::cout << "########################################################\nMission 1: External merge sort\n";
        std::cout << "########################################################\n\n";
        std::cout << "Input the file name: [0]Quit\n";
        std::cin >> command;
        // If the command is valid, exit the loop
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nCommand does not exist!\n";
            continue;
        }
        break;
        
    }
}

void Checkinputbin(ifstream &inputFile, string fileNum, bool &check) // 讀入input的txt
{
    
    string filename;
    filename = "pairs" + fileNum + ".bin";
    inputFile.open(filename);
    if (fileNum == "0"){
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


struct Connection{
	string sendID;
	string receiveID;
	float weight;
};

vector<Connection> datas ;
void merge(vector<Connection> &datas, int front, int mid, int end) {
	vector<Connection> tempL(datas.begin()+front, datas.begin()+mid+1),tempR(datas.begin()+mid+1,datas.begin()+end+1);
	Connection tempC;
	tempC.weight = -1;
	
	tempL.push_back(tempC);
	tempR.push_back(tempC);
	
	int indexL = 0, indexR = 0;
	
	for ( int i = front ; i <= end ; i++ ) {
		if ( tempL[indexL].weight >= tempR[indexR].weight ) {
			datas[i] = tempL[indexL];
			indexL++;
		}else{
			datas[i] = tempR[indexR];
			indexR++;
		}
	}
}

void mergeSort( vector<Connection>& datas, int front, int end ) {
	if ( front < end ) {
		int mid = (front+end) / 2 ;
		mergeSort(datas, front, mid);
		mergeSort(datas, mid+1, end);
		merge(datas, front, mid, end);
	}
}

int internalSort ( string fileNum, int maximum ) {
	datas.clear();
	bool eof = true;
	int fileNumInt = 0;
	string filename = "pairs"+fileNum+".bin";
	ifstream inputFile(filename, ios::binary);
	eof = inputFile.eof();
	while ( eof == false ) {
		fileNumInt++;
		datas.clear();
		for ( int i = 0 ; i < maximum ; i++ ) {
			
    		if (!inputFile.eof()) {
    			char sendID[10];
    			char receiveID[10];
    			float weight;
    			inputFile.read(sendID, 10);
    			if (inputFile.eof()){
    				eof = true;
    				break;
				}
    			inputFile.read(receiveID, 10);
    			inputFile.read(reinterpret_cast<char*>(&weight), sizeof(float));
		
		
				Connection tempCon ;
				tempCon.receiveID = receiveID;
				tempCon.sendID = sendID;  
				tempCon.weight = weight;
				datas.push_back(tempCon) ;
				
		                 
			//	cout << tempCon.receiveID << "----" << tempCon.sendID << "----" << tempCon.weight << '\n';
    		    
   	 		}else{
    			eof = true;
    			break;
			}
		}
		
//		cout << "hi";
		mergeSort(datas,0,datas.size()-1);
		
//		for ( int i = 0 ; i < datas.size() ; i++ ) {
//			cout << datas[i].sendID <<  " " <<datas[i].receiveID << " " << datas[i].weight <<  '\n';
//		}
		
		if ( datas.size() != 0 ) {
			stringstream ss;
			ss << fileNumInt;
			string tempOutName = "temp"+ss.str()+".bin";
			ofstream tempoutputFile(tempOutName,ios::binary);	
			for ( int i = 0 ; i < datas.size() ; i++ ) {
				tempoutputFile.write(datas[i].sendID.c_str(),10);
				tempoutputFile.write(datas[i].receiveID.c_str(),10);
				tempoutputFile.write(reinterpret_cast<char*>(&datas[i].weight), sizeof(float));
			}
			
			tempoutputFile.close();
			
		}
		
		
	}
	

	return fileNumInt;
	
}

//begin 以及 tempfileNum用來表示現在讀到第幾個暫存檔以及有幾個暫存檔要讀 
void externalSort( int tempfileNum, int begin, string fileNum ) {
	cout << "\nNow there are " << tempfileNum - begin + 1 << " runs.\n";
	if ( tempfileNum - 1 < begin ) {
		return;
	}
	
	//為了維持stable如果是基數的話需要做特別的處理，isodd代表上一次sort產生的檔案數量是否為基數 
	bool isOdd = false;
	
	if ( (tempfileNum - begin) % 2 == 0  ) {
		isOdd = true;
	}
	//newFileNum代表這次任務產生幾個檔案 
	int newFileNum = 0;
	while ( begin < tempfileNum ) {
		newFileNum++;
		//讀取上一次Sort所讀到的兩個檔案 
		string readFile1 = "temp" + to_string(begin++)+ ".bin";
		string readFile2 = "temp" + to_string(begin++) + ".bin";
		ifstream file1Stream(readFile1,ios::binary);
		ifstream file2Stream(readFile2,ios::binary);
		string outFile = "temp" + to_string(newFileNum+tempfileNum) + ".bin";
		//寫入新的檔案 
		ofstream outStream(outFile,ios::binary);
		bool file1End = false, file2End = false;
		Connection tempC1,tempC2; 
	
		//將檔案讀到的資料作暫存 
		char tempSend[10];
		char tempRece[10];
		float tempweight;
		file1Stream.read(tempSend,10);
		tempC1.sendID = tempSend;
		file1Stream.read(tempRece,10);
		file1Stream.read(reinterpret_cast<char*>(&tempweight), sizeof(float));
		tempC1.receiveID = tempRece;
		tempC1.weight = tempweight;
		
		
		file2Stream.read(tempSend,10);
		tempC2.sendID = tempSend;
		file2Stream.read(tempRece,10);
		file2Stream.read(reinterpret_cast<char*>(&tempweight), sizeof(float));
		tempC2.receiveID = tempRece;
		tempC2.weight = tempweight;
		//cout << "------------\n-------------\n------------\n"<<tempC1.sendID << " " << tempC2.sendID << '\n';
		//直到兩個檔案都讀完為止 
		while ( true ) {
			
		
			//假如其中一個先讀完直接把另一個檔案的資料全塞進去 
			if ( file1End ) {
				outStream.write(tempC2.sendID.c_str(),10);
				outStream.write(tempC2.receiveID.c_str(),10);
				outStream.write(reinterpret_cast<char*>(&tempC2.weight), sizeof(float));
				
				while (true) {
					char sendID[10];
					file2Stream.read(sendID,10);
					if ( file2Stream.eof() ) {
						break;
					}
					char receiveID[10];
					float weight1;
					file2Stream.read(receiveID,10);
					file2Stream.read(reinterpret_cast<char*>(&weight1), sizeof(float));
					outStream.write(sendID,10);
					outStream.write(receiveID,10);
					outStream.write(reinterpret_cast<char*>(&weight1), sizeof(float));
						
				}
				
				break;
			}
			
			
			if ( file2End ) {
				outStream.write(tempC1.sendID.c_str(),10);
				outStream.write(tempC1.receiveID.c_str(),10);
				outStream.write(reinterpret_cast<char*>(&tempC1.weight), sizeof(float));
				
				while (true) {
					char sendID[10];
					file1Stream.read(sendID,10);
					if ( file1Stream.eof() ) {
						break;
					}
					char receiveID[10];
					float weight1;
					file1Stream.read(receiveID,10);
					file1Stream.read(reinterpret_cast<char*>(&weight1), sizeof(float));
					outStream.write(sendID,10);
					outStream.write(receiveID,10);
					outStream.write(reinterpret_cast<char*>(&weight1), sizeof(float));	
					
				}
				
				break;
			}
			
			//比較兩個檔案讀到的資料 
			if ( tempC1.weight >= tempC2.weight ) {
				
				outStream.write(tempC1.sendID.c_str(),10);
				outStream.write(tempC1.receiveID.c_str(),10);
				outStream.write(reinterpret_cast<char*>(&tempC1.weight), sizeof(float));
				
				//cout << tempC1.weight << "-------------------\n";
				char sendID[10];
				file1Stream.read(sendID,10);
				if ( file1Stream.eof( )) {
					file1End = true;
				}else {
					char receiveID[10];
					float weight1 ;
					file1Stream.read(receiveID,10);
					file1Stream.read(reinterpret_cast<char*>(&weight1), sizeof(float));
					tempC1.sendID = sendID;
					tempC1.receiveID = receiveID;
					tempC1.weight = weight1;
				}
				
			}else{
				
				outStream.write(tempC2.sendID.c_str(),10);
				outStream.write(tempC2.receiveID.c_str(),10);
				outStream.write(reinterpret_cast<char*>(&tempC2.weight), sizeof(float));
				
				//cout << tempC2.weight << "-------------------\n";
				char sendID[10];
				file2Stream.read(sendID,10);
				if ( file2Stream.eof( )) {
					file2End = true;
				}else {
					char receiveID[10];
					float weight1 ;
					file2Stream.read(receiveID,10);
					file2Stream.read(reinterpret_cast<char*>(&weight1), sizeof(float));
					tempC2.sendID = sendID;
					tempC2.receiveID = receiveID;
					tempC2.weight = weight1;
				}	
			}
			
		}
		
		
		outStream.close();
		file1Stream.close();
		file2Stream.close();
		//cout << newFileNum << "----------------------\n";
	}
	
	//cout << tempfileNum + newFileNum << '\n';
	//假如只排列出一筆檔案代表是最後結果，將其更名並刪除暫存檔 
	if ( tempfileNum + newFileNum - begin == 0 ) {
		cout << "\nNow there are " <<  1 << " runs.\n";
		string tempfinalFile = "temp" + to_string(tempfileNum + newFileNum)+ ".bin";
		string finalFileName = "order" + fileNum + ".bin";
		remove(finalFileName.c_str());
		rename(tempfinalFile.c_str(),finalFileName.c_str());
		for ( int i = 1 ; i <= tempfileNum + newFileNum  ; i++ ) {
			string rmName = "temp" + to_string(i) + ".bin";
			remove(rmName.c_str());
		}
		
	}else{
		//如果前一次生成的檔案數量是基數，須將前一次最後一筆生成的檔案改成這次生成的檔案的最後面(為了保持stable 
		if ( isOdd ) {
			//cout << "hi\n";
			string nowFile = "temp" + to_string(begin++) + ".bin";
			string nextTempFile = "temp" + to_string(tempfileNum + ++newFileNum ) + ".bin";
			//cout << "\n" << nextTempFile << "\n" << begin << "\n";
			rename(nowFile.c_str(),nextTempFile.c_str());
		}
		externalSort(tempfileNum+newFileNum,begin,fileNum);	
	}
	
}

void mission1( string fileNum, int maximum ) {
	auto start = chrono::high_resolution_clock::now();
	int tempfileNum = internalSort(fileNum,maximum);
	auto end1 = chrono::high_resolution_clock::now();
	auto internalSortTime = chrono::duration_cast<chrono::milliseconds>(end1 - start);
	cout << "\nThe internal sort is completed. Check the initial sorted runs!\n";
	int nowFile = 1;
	externalSort(tempfileNum, nowFile,fileNum);
	auto end2 = chrono::high_resolution_clock::now();
	auto externalSortTime = chrono::duration_cast<chrono::milliseconds>(end2 - end1);
	auto totalTime = chrono::duration_cast<chrono::milliseconds>(end2 - start);
	cout << "\nThe execution time ...\n";
	cout << "Internal Sort = " << internalSortTime.count() << " ms\n";
	cout << "External Sort = " << externalSortTime.count() << " ms\n";
	cout << "Total Execution Time = " << totalTime.count() << " ms\n";
	
	
}

struct primary{
	float weight;
	int offset;
};
vector <primary> primaryIndex;

void mission2 ( string fileNum ) {
	primaryIndex.clear();
	cout << "\n\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
	cout << "Mission 2: Build the primary index\n";
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n";
	string orderFile = "order" + fileNum + ".bin";
	ifstream inputFile(orderFile, ios::binary );
	float nowWeight = -1;
	int offset = 0;
	int index = 1;
	cout << "\n<Primary index>: (key, offset)\n";
	while ( true ) {
		char tempA[10];
		char tempB[10];
		float weight;
		inputFile.read(tempA,10);
		if ( inputFile.eof() ) {
			break;
		}
		inputFile.read(tempB,10);
		inputFile.read(reinterpret_cast<char*>(&weight), sizeof(float));
		if ( weight != nowWeight ) {
		//	cout << "tempA: " << tempA << "\ttempB:" << tempB << '\n';
			nowWeight = weight;
			cout << '[' << index++ << "] (" << weight << ", " << offset << ")\n";
			primary tempP;
			tempP.offset = offset;
			tempP.weight = weight;
			primaryIndex.push_back(tempP);
		}
		
		
		offset++;
	}
	
	cout << "\n\n";
	
}

vector<Connection> mission3V;
void mission3(string fileNum) {
	mission3V.clear();
	cout << "\n\n\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
	cout << "\nMission 3: Threshold search on primary index\n";
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\nPlease input a threshold in the range [0,1]: \n";
	float threshold;
	cin >> threshold;
	auto start = chrono::high_resolution_clock::now();
	string inputFile = "order" + fileNum + ".bin";
	string outputFile = "order" + fileNum + ".txt";
	ifstream input(inputFile,ios::binary);
	remove(outputFile.c_str());
	ofstream output(outputFile);
	float tempWeight = primaryIndex[0].weight;
	int j = 0;
	//cout << "thre" << threshold <<endl;
	if ( tempWeight >= threshold ) {
		bool go = true;
		while ( go ) {
			if ( j > 0 ) {
				for ( int i = 0 ; i < primaryIndex[j].offset - primaryIndex[j-1].offset - 1 ; i++ ) {
				char tempC[10];
				float weight;
				input.read(tempC,10);
				input.read(tempC,10);
				input.read(reinterpret_cast<char*>(&weight), sizeof(float));
				
				}
				
				
			}
			
			char tempC1[10];
			char tempC2[10];
			float weight;
			input.read(tempC1,10);
			input.read(tempC2,10);
			input.read(reinterpret_cast<char*>(&weight), sizeof(float));
			if ( weight > threshold ) {
				Connection tempC;
				tempC.sendID = tempC1;
				tempC.receiveID = tempC2;
				tempC.weight = weight;
				mission3V.push_back(tempC);
			}else if( weight == threshold ) {
				Connection tempC;
				tempC.sendID = tempC1;
				tempC.receiveID = tempC2;
				tempC.weight = weight;
				mission3V.push_back(tempC);
				go = false;
			}
			
			
			
			j++;
		}
	}
	auto end1 = chrono::high_resolution_clock::now();
	auto totalTime = chrono::duration_cast<chrono::milliseconds>(end1 - start);
	output << "Search " << threshold << ":" << primaryIndex[j].offset << " records, " << totalTime.count() << "ms\n";
	for ( int i = 0 ; i < mission3V.size() ; i++ ) {
		output << "[" << primaryIndex[i].offset+1 << "]" << mission3V[i].sendID << " " << mission3V[i].receiveID << " " << mission3V[i].weight << '\n';
	}
	
	output.close();
	string tempS;
	cout << "\n\n\n[0]Quit or [Any other key]continue?\n";
	cin >> tempS;
	if ( strcmp(tempS.c_str(),"0") == 0 ) {
		return;
	}else{
		mission3(fileNum);
	}
	
}

int main()
{
    string fileNum ;
    GetCommand(fileNum);
    ifstream inputFile;
    ofstream outputFile;
    bool check = false;
    
    // 當指令不為0，就繼續讀取指令
    while ( fileNum.compare("0") != 0) {
        
        
        Checkinputbin(inputFile, fileNum, check);
            // 如果txt內沒東西 就結束
        if (inputFile.eof() || check == false ) {

    	} 
		else {
            mission1(fileNum,maximum);
            primaryIndex.clear();
			mission2(fileNum);
			mission3(fileNum); 
			
        }
        
        
        GetCommand(fileNum);
    }

    return 0;
}
