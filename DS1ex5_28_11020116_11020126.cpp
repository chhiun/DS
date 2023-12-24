// ?餉?銝?11020116 瞏?蝢? ?餉?銝?11020126 ?剔??
// 雿輻C++11

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
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
};

int main() {
    BSTree tree;
    ifstream inputFile;
    string filenum;

    int command;
    do {
    	GetCommand(command);

    	switch (command) {
        	case 0: // Quit
        	    cout << "Program terminates." << endl;
        	    break;
        	case 1: { // Read a file to build BST
            	bool check;
            	Inputtxt(inputFile, filenum, check);
            	if (check) {
                	vector<Pokemon> pokemons;
                	string line;

                	// Skip the header line
                	getline(inputFile, line);
	
                	while (getline(inputFile, line)) {
                	    pokemons.push_back(Pokemon::Parse(line));
                	}

                	// Displaying header
                	cout << "        #       Name                            Type 1          HP      Attack  Defense" << endl;

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
    	        break;
    	    }
    	}

	} while(command != 0) ;
    return 0;
}
