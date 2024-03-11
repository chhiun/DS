// 電資三 11020116 潘皓群
// 有使用C++11

#include <iostream>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue> 
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
    cout << "\nInput a file number ([0] Quit): ";
    cin >> filename;
    filenum = filename;
    filename = "input" + filename + ".txt";
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

struct Schooldata
{ // 用來各自存讀進的所有資料
    string schoolcode = "-1";
    string schoolname;
    string departcode;
    string departname;
    string type;
    string level;
    int studentnum = -1;
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

	int count = 0;
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



public:
    std::vector<Schooldata> heap;
    // 插入元素
    void insert(Schooldata data) {
        heap.push_back(data);
        heapifyUp(heap.size() - 1);
    }
    std::vector<Schooldata> getheap(){
    	return heap;
	}
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
    Schooldata getBottomdata() {
        if (heap.empty())
            cout << "Heap is empty\n";
        return heap.back();
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

	void clear(){
		heap.clear();
	}

};
class Heap {
private:

	int count = 0;
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void swap(Schooldata &a, Schooldata &b) {
        Schooldata temp;
		temp.studentnum = a.studentnum;
		temp.serialnum = a.serialnum;		
        a.studentnum = b.studentnum;
        a.serialnum = b.serialnum;
        b.studentnum = temp.studentnum;
        b.serialnum = temp.serialnum;
    }

    void MaxheapifyUp(int index) {
        while (index > 0 && heap[parent(index)].studentnum < heap[index].studentnum) {
            swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }
    void MinheapifyUp(int index) {
        while (index > 0 && heap[parent(index)].studentnum > heap[index].studentnum) {
            swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }


public:
    std::vector<Schooldata> heap;
    // 插入元素

    void rightinsert(Schooldata data) {
        heap.push_back(data);
        MaxheapifyUp(heap.size() - 1);
    }
    void leftinsert(Schooldata data) {
        heap.push_back(data);
        MinheapifyUp(heap.size() - 1);
    }
    
	void changetarget(int target, Schooldata data){
		
		heap[target] = data;
		MinheapifyUp(heap.size() - 1);

	}
	void exchangetarget(int target, Schooldata data){
		vector<Schooldata> save;
		for(int i = heap.size()-1; i > target; i--){
			save.push_back(heap[i]);
			heap.pop_back();
		}
		heap[target] = data;
		MinheapifyUp(heap.size() - 1);
		while(!save.empty()){
        	heap.push_back(save.back());
        	MinheapifyUp(heap.size() - 1);
        	save.pop_back();
		}

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
    Schooldata getBottomdata() {
        if (heap.empty())
            cout << "Heap is empty\n";
        return heap.back();
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

	void clear(){
		heap.clear();
	}

};
class Deap { //for 任務二   
private:
    Heap leftheap;
    Heap rightheap;
    int stage = 1;
    int countL = 0;
    int countR = 0;
    int position = 1;
    Schooldata bottom;
    Schooldata leftmost;
    bool check = false;

public:
    void insert(Schooldata data) {

    	int scale = pow(2, stage);

    	if(countL == 0){
    		leftheap.leftinsert(data);
    		bottom = leftheap.getBottomdata();
    		countL ++;
		}
    	else if(position <= (scale/2)){
    		if((data.studentnum > rightheap.getBottom()) && !rightheap.heap.empty()&&countL!=0){   			
    			Schooldata temp = rightheap.getBottomdata();
    			rightheap.heap.pop_back();
    			rightheap.rightinsert(data);
    			leftheap.leftinsert(temp);
			}
			else{
				leftheap.leftinsert(data);
			}
			countL++;
			bottom = leftheap.getBottomdata();
			if(position == 1){
				leftmost = leftheap.getBottomdata();
			}

		}
		else if(position > (scale/2)){
			if(data.studentnum < leftheap.heap[countR].studentnum){ // problem
				rightheap.rightinsert(leftheap.heap[countR]);
				leftheap.changetarget(countR, data);			
			}
			else{
				rightheap.rightinsert(data);
				
			}
			bottom = rightheap.getBottomdata();		
		}


		if(position == scale){
			stage++;
			position = 1;
		}
		else{
			position++;
		}

				 
    }
    
    int getBottom() {
        return bottom.studentnum;
    }
    int getBottomserial() {
        return bottom.serialnum;
    }

    int getLeftmostBottom() {

        return leftmost.studentnum;
    }
    int getLeftmostBottomserial() {

        return leftmost.serialnum;
    }
	void clear(){
		rightheap.clear();
		leftheap.clear();
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

                MaxHeap maxheap;
                for(int i = 0; i < alldata.size(); i ++){
                	maxheap.insert(alldata[i]);

				}
				cout << "<max heap>\n";
    			cout << "root: ["<< maxheap.getRootserial() <<"] " << maxheap.getRoot()<< endl;
    			cout << "bottom: ["<< maxheap.getBottomserial() <<"] " << maxheap.getBottom()<< endl;
    			cout << "leftmost bottom: ["<< maxheap.getLeftmostBottomserial() <<"] " << maxheap.getLeftmostBottom()<< endl;
    			maxheap.clear();


            }
            inputFile.close();
        }
        
		else if (command == 2) {
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

                Deap deap;
                for(int i = 0; i < alldata.size(); i ++){
                	deap.insert(alldata[i]);

				}
				cout << "<DEAP>\n";
    			cout << "bottom: ["<< deap.getBottomserial() <<"] " << deap.getBottom()<< endl;
    			cout << "leftmost bottom: ["<< deap.getLeftmostBottomserial() <<"] " << deap.getLeftmostBottom()<< endl;
    			deap.clear();


            }
            inputFile.close();
        }else {
            printf("Command does not exist!\n"); // 錯誤指令
        }
        GetCommand(command);
    }

    return 0;
}
