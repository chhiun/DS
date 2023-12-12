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
        std::cout << "*** University Graduate Information System ***\n";
        std::cout << "* 0. Quit                                    *\n";
        std::cout << "* 1. Create Two Binary Search Trees          *\n";
        std::cout << "* 2. Search by Number of Graduates           *\n";
        std::cout << "* 3. Search by School Name                   *\n";
        std::cout << "* 4. Removal by Number of Graduates          *\n";
        std::cout << "**********************************************\n";
        std::cout << "Input a command(0, 1-4): ";
        std::cin >> command;

        // If the command is valid, exit the loop
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nCommand does not exist!\n";
            continue;
        }
        if (command >= 0 && command <= 4) {
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
};

// 以vector建立Schooldata的List
class List
{
private:
    vector<Schooldata> alldata;

public:
    // 清空記憶體
    void clearlist()
    {
        alldata.clear();
    }

    // 加入list
    void add(Schooldata data)
    {
        alldata.push_back(data);
    }

    // 回傳資料數量
    int getsize()
    {
        return alldata.size();
    }

    // 以下都是獲取第i個資料的某項資訊
    Schooldata getdata(int i)
    {
        return alldata[i];
    }

    int getgraduate(int i)
    {
        Schooldata temp = alldata[i];

        return temp.graduatenum;
    }
    
    void deletetarget(int i){
        Schooldata temp = alldata[i];
        alldata.erase(alldata.begin()+i);
    }
    
    string getname(int i)
    {
        Schooldata temp = alldata[i];

        return temp.schoolname;
    }

    int getschoolcode(int i)
    {
        Schooldata temp = alldata[i];
        string target = temp.schoolcode;

        return stoi(target);
    }

    // 按照school code進行排序
    void sort()
    {
        for (int i = 0; i < alldata.size(); i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (stoi(alldata[i].schoolcode) < stoi(alldata[j].schoolcode))
                {
                    // 使用std::swap進行元素交換
                    std::swap(alldata[i], alldata[j]);
                }
            }
        }
    }

    void printlist()
    {
        Schooldata data;
        for (int i = 0; i < alldata.size(); i++) {
            data = alldata[i];
            cout << "[" << i + 1 << "]\t";
            // cout << data.schoolcode << "\t";
            cout << data.schoolname << "\t";
            // cout << data.departcode << "\t";
            cout << data.departname << "\t";
            cout << data.type << "\t";
            cout << data.level << "\t";
            cout << data.studentnum << "\t";
            cout << data.professornum << "\t";
            cout << data.graduatenum << "\t";
            // cout << data.city << "\t";
            // cout << data.system << "\n";
            cout << "\n";
        }
    }
};

// for建立BST的節點，每個節點存一個schooldata
template <typename T> class TreeNode
{
public:
    T data;
    Schooldata schooldata;
    TreeNode *left;
    TreeNode *right;
    TreeNode(T val, Schooldata target) : data(val), schooldata(target), left(nullptr), right(nullptr) {}
};

template <typename T> class BinarySearchTree
{
private:
    TreeNode<T> *root;

    
    TreeNode<T> *insert(TreeNode<T> *node, T value, Schooldata data)
    {
        if (node == nullptr) {
            return new TreeNode<T>(value, data);
        }

        if (value < node->data) {
            node->left = insert(node->left, value, data);
        } else if (value >= node->data) {
            node->right = insert(node->right, value, data);
        }

        return node;
    }

    // for任務二，找到的東西會存進List裡面
    void findGreater(TreeNode<T> *node, int target, List &result)
    {
        if (node == nullptr) {
            return;
        }

        if (node->data >= target) {
            result.add(node->schooldata);
        }

        if (target < node->data) {
            findGreater(node->left, target, result);
        }

        findGreater(node->right, target, result);
    }

    // for任務三，找到的東西會存進List裡面
    void findSame(TreeNode<T> *node, string target, List &result)
    {
        if (node == nullptr) {
            return;
        }

        if (node->data == target) {
            result.add(node->schooldata);
        }

        if (target < node->data) {
            findSame(node->left, target, result);
        }

        findSame(node->right, target, result);
    }
    void findless(TreeNode<T> *node, int target, List &result)
    {
        if (node == nullptr) {
            return;
        }

        if (node->data <= target) {
            result.add(node->schooldata);
        }

        if (target > node->data) {
            findless(node->right, target, result);
        }

        findless(node->left, target, result);
    }

    // 查詢樹高
    int height(TreeNode<T> *node)
    {
        if (node == nullptr) {
            return 0;
        }

        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        return 1 + std::max(leftHeight, rightHeight);
    }

    // 釋放空間
    void clear(TreeNode<T> *node)
    {
        if (node == nullptr) {
            return;
        }

        clear(node->left);
        clear(node->right);

        delete node;
    }
    
    TreeNode<T>* remove(TreeNode<T>*& node, T value) 
    {
        if (node == nullptr) {
            return nullptr;
        }

        if (value < node->data) {
            node->left = remove(node->left, value);
        } else if (value > node->data) {
            node->right = remove(node->right, value);
        } else {
            if (node->left == nullptr) {
                TreeNode<T>* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                TreeNode<T>* temp = node->left;
                delete node;
                return temp;
            }

            // Node with two children: Get the inorder successor (smallest
            // in the right subtree)
            TreeNode<T>* temp = minValueNode(node->right);

            // Copy the inorder successor's data to this node
            node->data = temp->data;

            // Delete the inorder successor
            node->right = remove(node->right, temp->data);
        }

        return node;
    }

    TreeNode<T>* minValueNode(TreeNode<T>* node) 
    {
        TreeNode<T>* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

public:
    // for外部呼叫
    BinarySearchTree() : root(nullptr) {}

    void insert(T value, Schooldata data)
    {
        root = insert(root, value, data);
    }

    List findGreater(int target)
    {
        List result;
        findGreater(root, target, result);

        return result;
    }
    List findless(int target)
    {
        List result;
        findless(root, target, result);

        return result;
    }
    List findSame(string target)
    {
        List result;
        findSame(root, target, result);

        return result;
    }

    int getHeight()
    {
        return height(root);
    }

    bool search(int value)
    {
        return search(root, value) != nullptr;
    }

    void clear()
    {
        clear(root);
        root = nullptr;
    }
    void remove(T value) 
    {
        root = remove(root, value);
    }
};

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
    data.studentnum = tokens[6].empty() ? 0 : stoi(tokens[6]); // 學生數
    data.professornum = tokens[7].empty() ? 0 : stoi(tokens[7]); // 教師數
    data.graduatenum = tokens[8].empty() ? 0 : stoi(tokens[8]); // 畢業生數
    data.city = tokens[9]; // 城市
    data.system = tokens[10]; // 系統別
}

int main()
{
    int command = 0;
    GetCommand(command);
    ifstream inputFile;
    ofstream outputFile;
    List datalist;
    BinarySearchTree<int> gradtree;
    BinarySearchTree<string> nametree;
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
                datalist.clearlist();
                gradtree.clear();
                // 每次重新執行command==1才把tree和list的空間放掉，不然都以上次執行的資料來做
                nametree.clear();
                //cout << s1 << "\n";
                //cout << s2 << "\n";
                //cout << "	學校名稱	科系名稱	日間∕進修別	等級別	學生數	教師數	上學年度畢業生數" << endl;
                // 用stringstream分割並把資料存進list
                while (!inputFile.eof()) {
                    Schooldata data;
                    getline(inputFile, s);
                    // 呼叫新的方法，並傳入分隔符號，這裡使用 '\t' 代表 Tab 分隔符號
                    StoreData(s, data, "\t");
                    // 避免放錯東西
                    if (data.schoolcode != "-1" && !data.schoolcode.empty()) {
                        datalist.add(data);
                    }
                }
                //datalist.printlist();

                for (int i = 0; i < datalist.getsize(); i++) {
                    // 以畢業人數為key建立一個Tree
                    gradtree.insert(datalist.getgraduate(i), datalist.getdata(i));
                }

                for (int i = 0; i < datalist.getsize(); i++) {
                    // 以學校名字為key建立一個Tree
                    nametree.insert(datalist.getname(i), datalist.getdata(i));
                }
                cout << "Tree height {School name} = " << nametree.getHeight() << "\n";
                cout << "Tree height {Number of graduates} = " << gradtree.getHeight() << "\n";
            }
            inputFile.close();
        } else if (command == 2) {
            if (gradtree.getHeight() == 0) {
                cout << "\nPlease choose command 1 first!\n";
            } else {
                List savelist;
                string input;
                int target = -1;
                bool check = true;
                cout << "Input the number of graduates:";
                cin >> input;

                // 檢查輸入字串是否為數字
                for (int i = 0; i < input.size(); i++) {
                    if (!isdigit(input[i])) {
                        check = false;
                    }
                }

                if (check == false) {
                    cout << "### the input string " << input << " is not a decimal number! ###\n";
                    cout << "There is no match!\n";
                } else {
                    target = stoi(input);
                    cout << "Search results:" << endl;
                    // 找到的目標會在這個List裡面
                    savelist = gradtree.findGreater(target);
                    if (savelist.getsize() == 0) {
                        cout << "There is no match!\n";
                    } else {
                        // savelist.sort(); //按school code排序
                        savelist.printlist();
                    }
                    savelist.clearlist();
                }
            }
        } else if (command == 3) {
            if (nametree.getHeight() == 0) {
                cout << "\nPlease choose command 1 first!\n";
            } else {
                List savelist;
                string target;
                cout << "Input a school name:";
                cin >> target;
                // 找到的目標會在這個List裡面
                savelist = nametree.findSame(target);
                if (savelist.getsize() == 0) {
                    cout << "There is no match!\n";
                } else {
                    // 按school code排序
                    savelist.sort();
                    savelist.printlist();
                }
                savelist.clearlist();
            }
        } else if (command == 4) {
            if (gradtree.getHeight() == 0) {
                cout << "\nPlease choose command 1 first!\n";
            } else {
                List savelist;
                string input;
                int target = -1;
                bool check = true;
                cout << "Input the number of graduates:";
                cin >> input;
                // 檢查輸入字串是否為數字
                for (int i = 0; i< input.size(); i ++) {
                    if (!isdigit(input[i])) {
                        check = false;
                    }
                }
                if (check == false) { 
                    cout<< "### the input string "<< input << " is not a decimal number! ###\n" ;
                    cout << "There is no match!\n";
                } else {
                    target = stoi(input);
                    List newlist;
                    // 將大於該目標的資料保留下來
                    for (int i = 0; i < datalist.getsize(); i++) {
                        if (datalist.getgraduate(i) >= target) {
                            newlist.add(datalist.getdata(i));
                        }
                    }
                    // for 印出 delete record
                    savelist = gradtree.findless(target);
                    datalist.clearlist();
                    // 將原本的資料替換為刪除後的資料
                    datalist = newlist;
                    if (savelist.getsize() == 0) {
                        cout << "There is no match!\n";
                    } else {
                        cout << "Deleted records:" << "\n"; 
                        savelist.printlist(); 
                    }
                    nametree.clear();
                    gradtree.clear();  
                    for (int i = 0; i < datalist.getsize(); i++) {
                        // 重新已刪除後的list建立，以畢業人數為key建立一個Tree
                        gradtree.insert(datalist.getgraduate(i), datalist.getdata(i));
                    }

                    for (int i = 0; i < datalist.getsize(); i++) {
                        // 重新已刪除後的list建立，以學校名字為key建立一個Tree
                        nametree.insert(datalist.getname(i), datalist.getdata(i));
                    }
                }
                cout << "Tree height {School name} = " << nametree.getHeight() << "\n";
                cout << "Tree height {Number of graduates} = "<< gradtree.getHeight() << "\n";
                savelist.clearlist();
            }
        } else {
            printf("Command does not exist!\n"); // 錯誤指令
        }
        GetCommand(command);
    }

    return 0;
}
