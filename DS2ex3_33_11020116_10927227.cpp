// 電資三 11020116 潘皓群  10927227 姜智紳 
// 有使用C++11

#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <cstring>
#include <vector>
#include <queue>
#include <string.h>
#include <stdlib.h>
using namespace std;

void GetCommand(int &command)
{
    while (true) {
        std::cout << "\n";
        std::cout << "******* Hash Table *****\n";
        std::cout << "* 0. QUIT              *\n";
        std::cout << "* 1. Quadratic probing *\n";
        std::cout << "* 2. Double hashing    *\n";
        std::cout << "************************\n";
        std::cout << "Input a command(0, 1, 2): ";
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
void Inputtxt(ifstream &inputFile, string &filenum, bool &check) // 讀入input的txt
{
    string filename;
    //cout << "\nInput a file number ([0] Quit): ";
    //cin >> filename;
    filename = "input" + filenum + ".txt";
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
void Inputbin(ifstream &inputFile, string &filenum, bool &check) // 讀入input的txt
{
    string filename;
    //cout << "\nInput a file number ([0] Quit): ";
    //cin >> filename;
    filename = "input" + filenum + ".bin";
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
struct Studentdata
{ // 用來各自存讀進的所有資料
	string sid = "-1";
	string name;
	vector<int> grades;
	float mean;
	
};
struct BinStudentdata
{ // 用來各自存讀進的所有資料
    char sid[10];
    char sname[10];
    unsigned char scores[6];
    float mean;
};
void printvector(vector<BinStudentdata> alldata)
{ //for檢查使用 
    for (int i = 0; i < alldata.size(); i++) {
        cout << alldata[i].sid << "\t";
        cout << alldata[i].sname << "\t";
        for(int j = 0; j < 6; j++){
        	cout << alldata[i].scores[j] << "\t";	
		}
		cout << alldata[i].mean << "\t";
        cout << "\n";
    }	
}




void BinStoreData(vector<BinStudentdata> &alldata, string filenum)
{
	BinStudentdata data;
	string line;
	ifstream inputFile;
	inputFile.open("input" + filenum + ".txt");
    
    while (getline(inputFile, line)) {
    	vector<string> tokens;
    	stringstream ss(line);
    	string token;
    	cout << ss;

        while (getline(ss, token, '\t')) {
            tokens.push_back(token);
        }

    	strcpy(data.sid, tokens[0].c_str());
    	strcpy(data.sname, tokens[1].c_str());
    	for (int i = 0; i < 6; i++) {
    		data.scores[i] = stoi(tokens[i+2]);
    	}
    	data.mean = stof(tokens[8]);
		alldata.push_back(data);	
		
    }
	

}

void mission0(string &filenum, vector<BinStudentdata> &alldata, bool check){
	ifstream inputFile;
	bool bin = true;
    cout << "\nInput a file number ([0] Quit): ";
    cin >> filenum;	
    Inputbin(inputFile, filenum, check);
    if((inputFile.eof() || check == false)&&filenum!="0"){
        Inputtxt(inputFile, filenum, check);
        bin = false;
	}
    // 如果txt內沒東西 就結束 
    if (inputFile.eof() || check == false ) {
    	//cout << "\n### Get nothing from the file input" << filenum << ".txt ! ###\n";
        //cout << "\nThere is no data!\n";
    } 
	else {                  
        BinStoreData(alldata, filenum);                 
    }
    inputFile.close();	
    if(bin == false){
    	ofstream outputFile;
    	string filename = "input" + filenum + ".bin";
    	outputFile.open(filename);
    	if(!outputFile){
        	cout << "\n###" << filename << " does not exist! ###" << endl;
        	cin.clear();   
			return; 		
		}
		else{
    		for (int i = 0; i < alldata.size(); i++) {
    		    outputFile << alldata[i].sid << "	" << alldata[i].sname << "	";
    		    for (int j = 0; j < 6; j++) {
    		        outputFile << (int)(alldata[i].scores[j]) << "	";
    		    }
    		    outputFile << alldata[i].mean << endl;
    		}
			outputFile.close();			
		}
	}
}



int main()
{
    int command = 0;
    GetCommand(command);
    ifstream inputFile;
    ofstream outputFile;
    string filenum;
    bool check = false;
    vector<BinStudentdata> alldata;


    // 當指令不為0，就繼續讀取指令
    while (command != 0) {
        if (command == 1) {    	
			mission0(filenum, alldata, check);
			printvector(alldata);
        }
		else {
            printf("Command does not exist!\n"); // 錯誤指令
        }
        GetCommand(command);
    }

    return 0;
}
