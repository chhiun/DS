// 電資三 11020116 潘皓群
// 有使用C++11

#include <iostream>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
    int studentnum = 0;
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
void Txtprintvector(vector<Schooldata> alldata, string filenum){
	ofstream outputFile;
	string filename = "output" + filenum + ".txt";
	outputFile.open(filename);
	if (!outputFile.is_open()) {
        cout << "Failed to open file.\n";
    }
    else{
	
    	for (int i = 0; i < alldata.size(); i++) {
        	//outputFile << "[" << alldata[i].serialnum << "]\t";
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



class MaxHeap {
private:
    std::vector<Schooldata> heap;

    // ?取父??索引
    int parent(int i) { return (i - 1) / 2; }

    // ?取左子??索引
    int leftChild(int i) { return 2 * i + 1; }

    // ?取右子??索引
    int rightChild(int i) { return 2 * i + 2; }

    // 交???元素
    void swap(Schooldata &a, Schooldata &b) {
        Schooldata temp;
		temp.studentnum = a.studentnum;
		temp.serialnum = a.serialnum;
		
        a.studentnum = b.studentnum;
        a.serialnum = b.serialnum;
        b.studentnum = temp.studentnum;
        b.serialnum = temp.serialnum;
    }

    // 向上?整堆
    void heapifyUp(int index) {
        while (index > 0 && heap[parent(index)].studentnum < heap[index].studentnum) {
            swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

    // 向下?整堆
    void heapifyDown(int index) {
        int maxIndex = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < heap.size() && heap[left].studentnum > heap[maxIndex].studentnum)
            maxIndex = left;

        if (right < heap.size() && heap[right].studentnum > heap[maxIndex].studentnum)
            maxIndex = right;

        if (index != maxIndex) {
            swap(heap[index], heap[maxIndex]);
            heapifyDown(maxIndex);
        }
    }

public:
    // 插入元素
    void insert(Schooldata data) {
        heap.push_back(data);
        heapifyUp(heap.size() - 1);
    }

    // ?取根??值
    int getRoot() {
        if (heap.empty())
            cout << "Heap is empty\n";
        return heap[0].studentnum;
    }
    int getRootserial() {
        if (heap.empty())
            cout << "Heap is empty\n";
        return heap[0].serialnum;
    }

    // ?取底部值
    int getBottom() {
        if (heap.empty())
            cout << "Heap is empty\n";
        return heap.back().studentnum;
    }
    int getBottomserial() {
        if (heap.empty())
            cout << "Heap is empty\n";
        return heap.back().serialnum;
    }
    // ?取最左下角??值
    int getLeftmostBottom() {
        if (heap.empty())
            cout << "Heap is empty\n";
        int index = 0;
        while (leftChild(index) < heap.size()) {
            index = leftChild(index);
        }
        return heap[index].studentnum;
    }
    int getLeftmostBottomserial() {
        if (heap.empty())
            cout << "Heap is empty\n";
        int index = 0;
        while (leftChild(index) < heap.size()) {
            index = leftChild(index);
        }
        return heap[index].serialnum;
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
                //cout << "\n### Get nothing from the file input" << filenum << ".txt ! ###\n";
                //cout << "\nThere is no data!\n";
            } else {
            	vector<Schooldata> alldata;
            	Schooldata bottom;
            	Schooldata leftmost;
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
                //Txtprintvector(alldata, filenum);
                MaxHeap maxheap;
                for(int i = 0; i < alldata.size(); i ++){
                	maxheap.insert(alldata[i]);

				}
				cout << "<max heap>\n";
    			cout << "root: ["<< maxheap.getRootserial() <<"] " << maxheap.getRoot()<< endl;
    			cout << "bottom: ["<< maxheap.getBottomserial() <<"] " << maxheap.getBottom()<< endl;
    			cout << "leftmost bottom: ["<< maxheap.getLeftmostBottomserial() <<"] " << maxheap.getLeftmostBottom()<< endl;


            }
            inputFile.close();
        }else {
            printf("Command does not exist!\n"); // 錯誤指令
        }
        GetCommand(command);
    }

    return 0;
}
