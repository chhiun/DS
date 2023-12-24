// 電資三 11020116 潘皓群  電資三 11020126 鄭祐昀 
// 有使用C++11

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> 
using namespace std;

void GetCommand(int &command)
{
    while (true) {
        std::cout << "\n";
        std::cout << "***** Pokemon Tree and Heap *****\n";
        std::cout << "* 0. QUIT                       *\n";
        std::cout << "* 1. Read a file to build BST   *\n";
        std::cout << "* 2. Transform it into Max Heap *\n";
        std::cout << "*********************************\n";
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

void Inputtxt(ifstream &inputFile, string &filenum, bool &check)
{
    string filename;
    while (filename != "0") {
        cout << "\nInput a file number [0: quit]: ";
        cin >> filename;
        if (filename == "0") {
            break;
        }
        filenum = filename;
        filename = "input" + filename + ".txt";
        inputFile.open(filename);
        if (!inputFile) {
            cout << "\n###" << filename << " does not exist! ###" << endl;
            cin.clear();
            check = false;
        } else {
            check = true;
            break;
        }
        inputFile.close();
    }
}

// 用來存pokemon的各項資訊 
class Pokemon {  
public:
    int number;
    std::string name;
    std::string type1;
    std::string type2;
    int total;
    int hp;
    int attack;
    int defense;
    int spAtk;
    int spDef;
    int speed;
    int generation;
    bool legendary;

    Pokemon() : number(0), total(0), hp(0), attack(0), defense(0), 
                spAtk(0), spDef(0), speed(0), generation(0), legendary(false) {}
	//用來從txt中讀取pokemon資訊 
    static Pokemon Parse(const std::string &line) {
        std::istringstream iss(line);
        Pokemon p;
        std::string token;
        std::getline(iss, token, '\t'); p.number = std::stoi(token);
        std::getline(iss, p.name, '\t');
        std::getline(iss, p.type1, '\t');
        std::getline(iss, p.type2, '\t');
        std::getline(iss, token, '\t'); p.total = std::stoi(token);
        std::getline(iss, token, '\t'); p.hp = std::stoi(token);
        std::getline(iss, token, '\t'); p.attack = std::stoi(token);
        std::getline(iss, token, '\t'); p.defense = std::stoi(token);
        std::getline(iss, token, '\t'); p.spAtk = std::stoi(token);
        std::getline(iss, token, '\t'); p.spDef = std::stoi(token);
        std::getline(iss, token, '\t'); p.speed = std::stoi(token);
        std::getline(iss, token, '\t'); p.generation = std::stoi(token);
        std::getline(iss, token, '\t'); p.legendary = (token == "TRUE");

        return p;
    }
};
//二元樹結構 
class BSTNode {
public:
    Pokemon data;
    size_t index;
    BSTNode *left;
    BSTNode *right;

    BSTNode(Pokemon p, size_t idx) : data(p), index(idx), left(nullptr), right(nullptr) {}
};

class BSTree {
private:
    BSTNode *root;

    void InsertNode(BSTNode *&node, Pokemon p, size_t idx) {
        if (!node) {
            node = new BSTNode(p, idx);
        } else if (p.hp >= node->data.hp) {
            InsertNode(node->right, p, idx);
        } else {
            InsertNode(node->left, p, idx);
        }
    }

    int Height(BSTNode *node) {
        if (!node) return 0;

        return 1 + std::max(Height(node->left), Height(node->right));
    }

    BSTNode *FindMin(BSTNode *node) {
        while (node && node->left) {
            node = node->left;
        }

        return node;
    }

    BSTNode *FindMax(BSTNode *node) {
        while (node && node->right) {
            node = node->right;
        }

        return node;
    }

    void ClearNode(BSTNode *node) {
        if (node) {
            ClearNode(node->left);
            ClearNode(node->right);
            delete node;
        }
    }

public:
    BSTree() : root(nullptr) {}

    void Insert(Pokemon p, size_t idx) {
        InsertNode(root, p, idx);
    }

    int TreeHeight() {
        return Height(root);
    }

    Pokemon LeftmostNode() {
        BSTNode *node = FindMin(root);
        if (node) return node->data;
        return Pokemon();  // return empty Pokemon if not found
    }

    size_t LeftmostNodeIndex() {
        BSTNode *node = FindMin(root);
        if (node) return node->index;
        return static_cast<size_t>(-1);  // return maximal size_t value as not found
    }

    Pokemon RightmostNode() {
        BSTNode *node = FindMax(root);
        if (node) return node->data;

        return Pokemon();  // return empty Pokemon if not found
    }

    size_t RightmostNodeIndex() {
        BSTNode *node = FindMax(root);
        if (node) return node->index;
        return static_cast<size_t>(-1);  // return maximal size_t value as not found
    }

    void Clear() {
        ClearNode(root);
        root = nullptr;
    }
};
//help!!!!!
void Heapify(vector<Pokemon> &pokemons, int n, int i) {
    int largest = i;
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    if (l < n && pokemons[l].hp > pokemons[largest].hp)
        largest = l;
    
    if (r < n && pokemons[r].hp > pokemons[largest].hp)
        largest = r;
    
    if ((l < n && pokemons[l].hp == pokemons[largest].hp && l < largest) ||
        (r < n && pokemons[r].hp == pokemons[largest].hp && r < largest)) {
        largest = (l < r) ? l : r;
    }
    
    if (largest != i) {
        swap(pokemons[i], pokemons[largest]);
        Heapify(pokemons, n, largest);
    }
}
//建立最大heap 
void BuildMaxHeap(vector<Pokemon> &pokemons) {
    int n = pokemons.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        Heapify(pokemons, n, i);
    }
}

int HeapHeight(int n) {
    return static_cast<int>(ceil(log2(n+1))); // n is the number of elements in the heap
}
// for 任務二 
void Task2(vector<Pokemon> &pokemons) {
	// build the max heap
    BuildMaxHeap(pokemons);

    // Output the max heap
    cout << "        #       Name                            Type 1          HP      Attack  Defense" << "\n";
    for (int i = 0; i < pokemons.size(); i++) {
		if(i<9){
			cout << "[ " << i+1 << "]\t";
		}
		else{
			cout << "[" << i+1 << "]\t";
		}
		cout << pokemons[i].number << "\t";
    	cout << pokemons[i].name << "\t\t\t";
		if(pokemons[i].name.length() < 8){
			cout << "\t";
		}    	                
    	cout << pokemons[i].type1 << "\t";
		if(pokemons[i].type1.length() < 8){
			cout << "\t";
		}
    	cout << pokemons[i].hp << "\t";
    	cout << pokemons[i].attack << "\t";
    	cout << pokemons[i].defense << endl;
    }

    // Calculate heap height
    int height = HeapHeight(pokemons.size());

    int leftmostIndex = (1 << (height - 1)) - 1;
    if (leftmostIndex >= pokemons.size()) {
        leftmostIndex = (1 << (height - 2)) - 1;
    }
    cout << "HP heap height = " << height << endl;

    // Output leftmost node and bottom node
    cout << "Leftmost node:\n[" << leftmostIndex << "]\t";
    cout << pokemons[leftmostIndex].number << "\t";
    cout << pokemons[leftmostIndex].name << "\t\t\t";
	if(pokemons[leftmostIndex].name.length() < 8){
		cout << "\t";
	} 
    cout << pokemons[leftmostIndex].type1 << "\t";
	if(pokemons[leftmostIndex].type1.length() < 8){
		cout << "\t";
	} 
    cout << pokemons[leftmostIndex].hp << "\t";
    cout << pokemons[leftmostIndex].attack << "\t";
    cout << pokemons[leftmostIndex].defense << endl;

    cout << "Bottom:\n[" << pokemons.size() - 1 << "]\t";
    
    const Pokemon &bottom = pokemons.back();
    cout << bottom.number << "\t";
    cout << bottom.name << "\t\t\t";
	if(bottom.name.length() < 8){
		cout << "\t";
	} 
    cout << bottom.type1 << "\t";
	if(bottom.type1.length() < 8){
		cout << "\t";
	} 
    cout << bottom.hp << "\t";
    cout << bottom.attack << "\t";
    cout << bottom.defense << endl;   
	 
}

int main() {
    BSTree tree;
    ifstream inputFile;
    string filenum;
    vector<Pokemon> pokemons;
	bool done = false;
    int command;

    do {
        GetCommand(command);
        switch (command) {
            case 0: // Quit

                break;
            case 1:
                bool check;
                pokemons.clear();
                tree.Clear();
                Inputtxt(inputFile, filenum, check);
                if (check) {
                    string line;

                    // Skip the header line
                    getline(inputFile, line);
            		if(inputFile.eof() || check == false){ // 如果txt內沒東西 就結束 
            			if(inputFile.eof()){
            				cout << "\n### Get nothing from the file input" << filenum << ".txt ! ###\n" ;
            				inputFile.close();
            				done = false;
							break;	
						}
					}

                    while (getline(inputFile, line)) {
                        pokemons.push_back(Pokemon::Parse(line));
                    }
                	

                    inputFile.close();

                    // Displaying header
                    cout << "    " << "# Name Type 1 HP Attack Defense" << endl;

                    // Displaying pokemons
                	for (size_t i = 0; i < pokemons.size(); ++i) {
						if(i<9){
							cout << "[ " << i+1 << "]\t";
						}
						else{
							cout << "[" << i+1 << "]\t";
						}
						cout << pokemons[i].number << "\t";
    	                cout << pokemons[i].name << "\t\t\t";
						if(pokemons[i].name.length() < 8){
							cout << "\t";
						}    	                
    	                cout << pokemons[i].type1 << "\t";
						if(pokemons[i].type1.length() < 8){
							cout << "\t";
						}
    	                cout << pokemons[i].hp << "\t";
    	                cout << pokemons[i].attack << "\t";
    	                cout << pokemons[i].defense << endl;

    	                // Inserting into BST with index
    	                tree.Insert(pokemons[i], i);
    	            }

                    // Display the result
                    cout << "HP tree height = " << tree.TreeHeight() << endl;
                    Pokemon leftmost = tree.LeftmostNode();
                    size_t leftIndex = tree.LeftmostNodeIndex();
    	            cout << "Leftmost node:\n";
    	            cout << "        #       Name                            Type 1          HP      Attack  Defense" << "\n";

					if(leftIndex<9){
						cout << "[ " << leftIndex+1 << "]\t";
					}
					else{
						cout << "[" << leftIndex+1 << "]\t";
					}
					cout << leftmost.number << "\t";
    	            cout << leftmost.name << "\t\t\t";
					if(leftmost.name.length() < 8){
						cout << "\t";
					}    	                
    	            cout << leftmost.type1 << "\t";
					if(leftmost.type1.length() < 8){
						cout << "\t";
					}
    	            cout << leftmost.hp << "\t";
    	            cout << leftmost.attack << "\t";
    	            cout << leftmost.defense << endl;
    	                


    	            Pokemon rightmost = tree.RightmostNode();
    	            size_t rightIndex = tree.RightmostNodeIndex();
    	            cout << "Rightmost node:\n";
    	            cout << "        #       Name                            Type 1          HP      Attack  Defense" << "\n";
					if(rightIndex<9){
						cout << "[ " << rightIndex+1 << "]\t";
					}
					else{
						cout << "[" << rightIndex+1 << "]\t";
					}
					cout << rightmost.number << "\t";
    	            cout << rightmost.name << "\t\t\t";
					if(rightmost.name.length() < 8){
						cout << "\t";
					}    	                
    	            cout << rightmost.type1 << "\t";
					if(rightmost.type1.length() < 8){
						cout << "\t";
					}
    	            cout << rightmost.hp << "\t";
    	            cout << rightmost.attack << "\t";
    	            cout << rightmost.defense << endl;
                }
                done = true;
                break;
            case 2:
            	if(done == true){
            		Task2(pokemons);
				}
				else{
					cout << endl << "----- Execute Mission 1 first! -----" << endl;
				}
                break;
        }
    } while (command != 0);

    return 0;
}
