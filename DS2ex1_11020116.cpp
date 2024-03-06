// 電資三 11020116 潘皓群  電資三 11020126 鄭祐昀
// 有使用C++11

#include <iostream>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void GetCommand(int &command)
{
    while (true) {
        std::cout << "\n";
        std::cout << "**** Heap Construction *****\n";
        std::cout << "* 0. QUIT                  *\n";
        std::cout << "* 1. Build a max heap      *\n";
        std::cout << "* 2. Build a DEAP          *\n";
        std::cout << "****************************\n";
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
    cout << "\nInput a file number: ";
    cin >> filename;
    filenum = filename;
    filename = "input" + filename + ".txt";
    inputFile.open(filename);
    if (!inputFile) {
        cout << "\n###" << filename << " does not exist! ###" << endl;
        cin.clear();
        check = false;
    } else {
        check = true;
    }
}

struct Schooldata
{ // 用來各自存讀進的所有資料
    string schoolcode = "-1";
    string schoolname;
    string departcode;
    string departname;
    string type;
    string level;
    int studentnum;
    int professornum;
    int graduatenum;
    string city;
    string system;
    int serialnum;
};


void printvector(vector<Schooldata> alldata){
    for (int i = 0; i < alldata.size(); i++) {
        cout << "[" << alldata[i].serialnum << "]\t";
        // cout << alldata[i].schoolcode << "\t";
        cout << alldata[i].schoolname << "\t";
        // cout << alldata[i].departcode << "\t";
        cout << alldata[i].departname << "\t";
        cout << alldata[i].type << "\t";
        cout << alldata[i].level << "\t";
        cout << alldata[i].studentnum << "\t";
        cout << alldata[i].professornum << "\t";
        cout << alldata[i].graduatenum << "\t";
        // cout << alldata[i].city << "\t";
        // cout << alldata[i].system << "\n";
        cout << "\n";
    }	
}
void Txtprintvector(vector<Schooldata> alldata){
	ofstream outputFile;
	outputFile.open("output.txt");
	if (!outputFile.is_open()) {
        cout << "Failed to open file.\n";
    }
    else{
	
    	for (int i = 0; i < alldata.size(); i++) {
        	outputFile << "[" << alldata[i].serialnum << "]\t";
        	//outputFile << alldata[i].schoolcode << "\t";
        	//outputFile << alldata[i].schoolname << "\t";
        	//outputFile << alldata[i].departcode << "\t";
        	//outputFile << alldata[i].departname << "\t";
        	//outputFile << alldata[i].type << "\t";
        	//outputFile << alldata[i].level << "\t";
        	outputFile << alldata[i].studentnum << "\t";
        	//outputFile << alldata[i].professornum << "\t";
        	//outputFile << alldata[i].graduatenum << "\t";
        	//outputFile << alldata[i].city << "\t";
        	//outputFile << alldata[i].system << "\n";
        	outputFile << "\n";
    	}
	}
	outputFile.close();	
}
string removeCommas(string str) {
    str.erase(remove(str.begin(), str.end(), ','), str.end());
    str.erase(remove(str.begin(), str.end(), '\"'), str.end());

    return str;
}

void StoreData(const string &line, Schooldata &data, const string &separator)
{
    vector<string> tokens;
    stringstream ss(line);
    string token;
    
    // 使用 getline 配合 separator 中的分隔符把每一項資料分割開來
    while (getline(ss, token, separator[0])) {
        tokens.push_back(token);
    }

    // 如果 tokens 的數目不夠，則將剩餘的欄位填充為空白
    while (tokens.size() < 11) {
        tokens.push_back("");
    }

    // 為 Schooldata 中的每個成員賦值，待會填充空值
    data.schoolcode = tokens[0];
    data.schoolname = tokens[1];
    data.departcode = tokens[2];
    data.departname = tokens[3];
    data.type = tokens[4]; // 日間 / 進修別
    data.level = tokens[5]; // 等級別
    string studentStr = removeCommas(tokens[6]);
    data.studentnum = studentStr.empty() ? 0 : stoi(studentStr); // 學生數
    string professorStr = removeCommas(tokens[7]);
    data.professornum = professorStr.empty() ? 0 : stoi(professorStr); // 教師數
    string graduateStr = removeCommas(tokens[8]);
    data.graduatenum = graduateStr.empty() ? 0 : stoi(graduateStr); // 畢業生數
    data.city = tokens[9]; // 城市
    data.system = tokens[10]; // 系統別
}

void Heapify(vector<Schooldata> &data, int n, int i) {
    int largest = i;
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    if (l < n && data[l].studentnum > data[largest].studentnum)
        largest = l;
    
    if (r < n && data[r].studentnum > data[largest].studentnum)
        largest = r;
    
    if ((l < n && data[l].studentnum == data[largest].studentnum && l < largest) ||
        (r < n && data[r].studentnum == data[largest].studentnum && r < largest)) {
        largest = (l < r) ? l : r;
    }
    
    if (largest != i) {
        swap(data[i], data[largest]);
        Heapify(data, n, largest);
    }
}
//建立最大heap 
void BuildMaxHeap(vector<Schooldata> &data) {
    int n = data.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        Heapify(data, n, i);
    }
}
int HeapHeight(int n) {
    return static_cast<int>(ceil(log2(n+1))); // n is the number of elements in the heap
}

int main()
{
    int command = 0;
    GetCommand(command);
    ifstream inputFile;
    ofstream outputFile;
    string filenum;
    bool check = false;

    // 當指令不為0，就繼續讀取指令
    while (command != 0) {
        if (command == 1) {
            Inputtxt(inputFile, filenum, check);
            string s;
            string s1, s2;
            getline(inputFile, s1);
            getline(inputFile, s2);
            getline(inputFile, s);
            s.clear();
            // 如果txt內沒東西 就結束
            if (inputFile.eof() || check == false) {
                if (inputFile.eof()) {
                    cout << "\n### Get nothing from the file input" << filenum << ".txt ! ###\n";
                }
                cout << "\nThere is no data!\n";
            } else {
            	vector<Schooldata> alldata;
            	int count = 1;
                while (!inputFile.eof()) {
                    Schooldata data;
                    getline(inputFile, s);
                    // 呼叫新的方法，並傳入分隔符號，這裡使用 '\t' 代表 Tab 分隔符號
                    StoreData(s, data, "\t");
                    data.serialnum = count;
                    count++;
                    // 避免放錯東西
                    if (data.schoolcode != "-1" && !data.schoolcode.empty()) {
                    	alldata.push_back(data);
                    }
                }
                //Txtprintvector(alldata);
                vector<Schooldata> Maxheap = alldata;
                BuildMaxHeap(Maxheap);
				Txtprintvector(Maxheap);	
				int height = HeapHeight(Maxheap.size());
				cout <<	Maxheap.size();
        		int leftmostIndex = (1 << (height - 1)) -1;
    			if (leftmostIndex >= alldata.size()) {
        			leftmostIndex = (1 << (height - 2)) -1;
    			}

    			cout << "root: ["<< Maxheap.front().serialnum <<"] " << Maxheap.front().studentnum<< endl;
    			cout << "bottom: ["<< Maxheap.back().serialnum <<"] " << Maxheap.back().studentnum<< endl;
    			cout << "leftmost bottom: ["<< Maxheap[leftmostIndex].serialnum <<"] " << Maxheap[leftmostIndex].studentnum<< endl;


            }
            inputFile.close();
        }else {
            printf("Command does not exist!\n"); // 錯誤指令
        }
        GetCommand(command);
    }

    return 0;
}
