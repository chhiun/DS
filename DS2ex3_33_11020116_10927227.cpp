// 電資三 11020116 潘皓群  10927227 姜智紳 
// 有使用C++11

#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
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




void StoreData(vector<BinStudentdata> &alldata, string filenum, bool bin)
{
	
	BinStudentdata data;
	string line;
	ifstream inputFile;
	if(bin == true){
		inputFile.open("input" + filenum + ".bin");
	}
	else if(bin == false){
		inputFile.open("input" + filenum + ".txt");
	}
	
    
    while (getline(inputFile, line)) {
    	vector<string> tokens;
    	stringstream ss(line);
    	string token;


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

void mission0(string &filenum, vector<BinStudentdata> &alldata){
	ifstream inputFile;
	bool bin = false;
	bool check = false;
    cout << "\nInput a file number ([0] Quit): ";
    cin >> filenum;	
    Inputbin(inputFile, filenum, check);
    if(check == true){
    	bin = true;
    	// 如果找到.bin 就用.bin來讀資料 
    	StoreData(alldata, filenum,bin); 
		return;  
	}
    else if((inputFile.eof() || check == false)&&filenum!="0"){
    	//沒找到.bin就去找找看有沒有.txt 
    	bin = false;
        Inputtxt(inputFile, filenum, check);
	}
    // 如果txt內沒東西 就結束 
    if (inputFile.eof() || check == false ) {
    	//cout << "\n### Get nothing from the file input" << filenum << ".txt ! ###\n";
        //cout << "\nThere is no data!\n";
    } 
	else {
		//如果.txt裡面有資料就存入資料並將其寫成.bin檔                  
        StoreData(alldata, filenum,bin);
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
    inputFile.close();	
}

bool IsPrime(int n) {
    if (n < 2) {
        return false;
    }
 
    for (int i = 2; i < n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
 
    return true;
}
struct M1 {
	
	BinStudentdata data;
	bool is_empty;
	int hashValue;
			

};
class quadratic_Hash{
	public:
		vector<BinStudentdata> alldata;
		string fileNum;
		int size = 0;
		
	int find_prime(){
		int size = alldata.size()*1.15;
		while(!IsPrime(size)){
			size++;
		}
		return size;
			
	} // find_prime
	int getHash( char sid[10]){
		unsigned long long step = 1;
		for ( int i = 0 ; i < sizeof(sid) / sizeof(sid[0]) ; i++ ) {
			if ( (int)sid[i] != 0 )
				step *= (int)sid[i];
		}
		
		
		step %= size;
		
		
		return step;
	}
    int quadraticProbe(int hashIndex, int i) {
        return (hashIndex + (i * i)) % size;
    }	
	void createTable(){
		size = find_prime();
		M1 hashTable[size];
		int count = 0;
		for (int i = 0; i < size ; i++ ) {
			//初始化hashTable 
			hashTable[i].is_empty = true;	
		}
		
		for ( int i = 0 ; i < alldata.size() ; i++ ) {
			int step = 0;
			int tempHash = getHash(alldata[i].sid);
			int hash = tempHash;	
			while (hashTable[tempHash].is_empty == false ) {
				tempHash = hash;
				tempHash = quadraticProbe(tempHash, step);		
				step++;
				count++;				
			}
			count++;
			hashTable[tempHash].data = alldata[i];
			hashTable[tempHash].is_empty = false;
			hashTable[tempHash].hashValue = hash;		
		}
		//cout << count << "\t" << alldata.size() << "\t" << size << endl;
		float unaverageComparisons = count / (float)size;
		float averageComparisons = count / (float)alldata.size();
		outputTable( size, hashTable );
		cout << "Hash table has been successfully created by Quadratic probing\n";
		cout << "unsuccessful search: " <<  fixed  <<  setprecision(4) << unaverageComparisons << " comparisons on average\n";
		cout << "successful search: " <<  fixed  <<  setprecision(4) << averageComparisons << " comparisons on average\n";
}
	void outputTable(int size,  M1 *m1  ){
		string final_file_num;
		final_file_num = "quadratic" + fileNum + ".txt" ;
		ofstream file(final_file_num, ios::out);  
		file << " --- Hash table created by Quadratic probing --- " << '\n' ;
		
		for(int i = 0; i < size ; i++ ) {
			
			file << '[' << i << ']'<< '\t';
			if ( m1[i].is_empty == true ) 
			 	file  << '\n';
			else 
				file  << m1[i].hashValue <<','<< '\t'  << m1[i].data.sid <<',' << '\t' << m1[i].data.sname << ',' << '\t'<< m1[i].data.mean<< '\n' ;
		}
		file << "----------------------------------------------" << '\n' ;
		file.close();
	}//writeM1			
};
class double_Hash{
	public:
		vector<BinStudentdata> alldata;
		string fileNum;
		int largestStep = 0 ;
		int size = 0;
	
	int find_prime(){
		int size = alldata.size()*1.15 + 1;
		
		int i = 0;
		while(true){
			for(i=2; i < size/2; i++) {
				if(size%i==0){
					break;
				}
			}
			if(i>=size/2){
				return size;
				break;
			}
			size++;
		}
			
	} // find_prime
	
	void getLargestStep() {
		int tempStep = (alldata.size() / 5) + 1 ;
		int i = 0;
		while(true){
			for(i=2; i < tempStep/2; i++) {
				if(tempStep%i==0){
					break;
				}
			}
			if( i >= tempStep/2){
				
				largestStep = tempStep;
				break;
			}
			tempStep++;
		}
	}
	
	int getStep( char sid[10]){
		unsigned long long step = 1;
		for ( int i = 0 ; i < sizeof(sid) / sizeof(sid[0]) ; i++ ) {
			if ( (int)sid[i] != 0 )
				step *= (int)sid[i];
			
		}
		
		
		step %= largestStep;
		
		
		return largestStep - step;
	}
	
	int getHash( char sid[10]){
		unsigned long long step = 1;
		for ( int i = 0 ; i < sizeof(sid) / sizeof(sid[0]) ; i++ ) {
			if ( (int)sid[i] != 0 )
				step *= (int)sid[i];
		}
		
		
		step %= size;
		
		
		return step;
	}
	
	void outputTable(int size,  M1 *m1  ){
		string final_file_num;
		final_file_num = "double" + fileNum + ".txt" ;
		ofstream file(final_file_num, ios::out);  
		file << " --- Hash Table created by Double hashing --- " << '\n' ;
		
		for(int i = 0; i < size ; i++ ) {
			
			file << '[' << i << ']'<< '\t';
			if ( m1[i].is_empty == true ) 
			 	file  << '\n';
			else 
				file  << m1[i].hashValue <<','<< '\t'  << m1[i].data.sid <<',' << '\t' << m1[i].data.sname << ',' << '\t'<< m1[i].data.mean<< '\n' ;
		}
		file << "----------------------------------------------" << '\n' ;
		file.close();
	}//writeM1	
	
    
	void createTable(){
		size = find_prime();
		
		getLargestStep();
		M1 hashTable[size];
		int count = 0;
		for (int i = 0; i < size ; i++ ) {
			hashTable[i].is_empty = true;
		
		}
		
		
		for ( int i = 0 ; i < alldata.size() ; i++ ) {
			
			int tempHash = getHash(alldata[i].sid);
			int hash = tempHash;
			int step = getStep(alldata[i].sid);
			
			while (hashTable[tempHash].is_empty == false ) {
				
				count++;
				tempHash += step;
				if ( tempHash >= size ){
					tempHash -= size;
					
				}
			}
			
			count++;
			
			hashTable[tempHash].data = alldata[i];
			hashTable[tempHash].is_empty = false;
			hashTable[tempHash].hashValue = hash;	
			
			
			
		}
		
		float averageComparisons = count / (float)alldata.size();
		outputTable( size, hashTable );
		cout << "Hash table has been successfully created by Double hashing\n";
		cout << "successful search: " <<  fixed  <<  setprecision(4) << averageComparisons << " comparisons on average\n";
		

		
	}
};
int main()
{
    int command = 0;
    GetCommand(command);
    string filenum;
    bool check = false;
    vector<BinStudentdata> alldata;


    // 當指令不為0，就繼續讀取指令
    while (command != 0) {
        if (command == 1) {    
			alldata.clear();	
			mission0(filenum, alldata);
			if(alldata.size()!=0){
				quadratic_Hash quadratic;
        		quadratic.alldata = alldata;
        		quadratic.fileNum = filenum;
				quadratic.createTable();			
				//printvector(quadratic.alldata);				
			}
			
        }
        else if ( command == 2) {
        	if(alldata.size()==0){
        		cout << "### Command 1 first. ###\n";

			}
			else{
        		cout << "\n\n";
        		double_Hash doubleHash;
        		doubleHash.alldata = alldata;
        		doubleHash.fileNum = filenum;
        		doubleHash.createTable();				
			}
		}
		else {
            printf("Command does not exist!\n"); // 錯誤指令
        }
        GetCommand(command);
    }

    return 0;
}
