#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
//#include "BST.cpp"
using namespace std;

struct BSTNode {
	int key;
	BSTNode *left, *right;
};

class HashTable {
private:
	int TABLE_SIZE;
	BSTNode* *table;
	int hash1(int k); //h(x) function
  int hash2(int k); //h'(x) function
	BSTNode* createNode(int k);
	BSTNode* insertNodeHelper(BSTNode* root, BSTNode* newNode);
	BSTNode* searchKeyHelper(BSTNode* root, int k);
	BSTNode* minNode(BSTNode* r);
	BSTNode* deleteNodeHelper(BSTNode* r, int k);
public:
	HashTable();
	bool insert(int key, int h);
  BSTNode* search(int key, int h);
  bool deleteNode(int key, int h);

};

HashTable::HashTable() {
	TABLE_SIZE = 10009;
	table = new BSTNode*[TABLE_SIZE];
	for (int i=0; i<TABLE_SIZE; i++)
		table[i] = nullptr; //initialize all indeces to null
}

int HashTable::hash1(int k) { //h(x) function
	return k % TABLE_SIZE;
}

int HashTable::hash2(int k) { //h'(x) function
	return (int)(floor(k/TABLE_SIZE)) % TABLE_SIZE;
}

BSTNode* HashTable::createNode(int k) {
	BSTNode* newNode = new BSTNode;
	newNode->key = k;
	newNode->left = nullptr;
	newNode->right = nullptr;
	return newNode;
}

BSTNode* HashTable::insertNodeHelper(BSTNode* r, BSTNode* newNode) {
	// Insert at the head
	if (r == nullptr)
		r = newNode;
	else if (r->key < newNode->key) // insert to the RIGHT
		r->right = insertNodeHelper(r->right, newNode);
	else // insert to left
		r->left = insertNodeHelper(r->left, newNode);
	//return new root
	return r;
}

bool HashTable::insert(int key, int h){
	//get index based on given hash function
  int index;
  if(search(key, h) == nullptr){
    if(h == 1){
      index = hash1(key);
    }else{
      index = hash2(key);
    }
		//create new node and insert, with table[index] as the root
		BSTNode* newNode = createNode(key);
    table[index] = insertNodeHelper(table[index], newNode);
    return true;
  }
  return false;
}

BSTNode* HashTable::searchKeyHelper(BSTNode* root, int k) {
	if (root) {
		if (root->key == k) //key found, return
			return root;
		else if (root->key < k) // Look to the right
			return searchKeyHelper(root->right, k);
		else if (root->key > k) // Look to the left
			return searchKeyHelper(root->left, k);
	}
	//key not found
	return nullptr;
}

BSTNode* HashTable::search(int key, int h){
	//get index based on given hash function
  int index;
  if(h == 1){
    index = hash1(key);
  }else{
    index = hash2(key);
  }
  if(table[index] == nullptr){
		//index is empty
    return nullptr;
  }
  BSTNode* n = searchKeyHelper(table[index], key);
  return n;
}

BSTNode* HashTable::minNode(BSTNode* r) {
	if (r->left)
		return minNode(r->left);
	if (!r->left)
		return r;
}

BSTNode* HashTable::deleteNodeHelper(BSTNode* r, int k) {
	// Insert at the head
	if (!r) // R is null
		return r;
	else if (r->key < k) // delete from the RIGHT
		r->right = deleteNodeHelper(r->right, k);
	else if (r->key > k) // delete from the LEFT
		r->left = deleteNodeHelper(r->left, k);
	else { // node to be deleted has been found
		// 3 Cases
		BSTNode* toReturn = r;
		if (r->left == r->right) { //NO CHILDREN
			delete r;
			r = nullptr;
		} else if (r->left == nullptr) { //delete right child
			toReturn = r->right;
			delete r;
			return toReturn;
		} else if (r->right == nullptr) { //delete left child
			toReturn = r->left;
			delete r;
			return toReturn;
		} else { //two children
			BSTNode* rightMin = minNode(r->right);
			int key = rightMin->key;
			deleteNodeHelper(r, rightMin->key);
			//replace node with rightMin
			rightMin->key = key;
			rightMin->left = r->left;
			rightMin->right = r->right;
			delete r;
			return rightMin;
		}
	}
	return r;
}

bool HashTable::deleteNode(int key, int h){
  if(search(key, h) == nullptr){
		//key isnt found anywhere, nothing to delete
    return false;
  }
	//get index based on given hash function
  int index;
  if(h == 1){
    index = hash1(key);
  }else{
    index = hash2(key);
  }
	//index is empty, cannot delete anything
	if(table[index] == nullptr) return false;
	//delete key
	table[index] = deleteNodeHelper(table[index], key);
  return true;
}

int main(int argc, char* argv[]){
	HashTable ht;
  ifstream fs;
  string inputFile = argv[1], x;
	int hfunc = stoi(argv[2]); //hash function to be used
  float lf = stof(argv[3]); //load factor
	int load = lf * 10000; //number of ints to load
  int arr1[load];
  int arr2[100];
  fs.open(inputFile);

  //insert appropriate number of ints from CSV file
    //also store into arr1[] for use in next part
  for(int i = 0; i < load; i++){
    getline(fs, x, ',');
    arr1[i] = stoi(x);
    ht.insert(arr1[i], hfunc);
  }

  //pick 100 random numbers stored in arr1[] to store in arr2[]
    //arr2[] will be used to test functions with 100 random
    //ints from the CSV file w/o having to access the file itself
    //for every single test
  int r = -1;
  for(int i = 0; i < 100; i++){
    while(arr1[r] == -1){
      r = ((int)(rand()) % load);
    }
    arr2[i] = arr1[r];
  }
  fs.close();

  int startTime, endTime;
  double execTime, avgInsertTime, avgSearchTime, avgDeleteTime;
  cout << "BST CHAINING" << endl;
	cout << "\tLOAD FACTOR: " << lf << "\tHASH FUNCTION " << hfunc << endl;

	//record DELETE times
	startTime = clock();
	for(int i = 0; i < 100; i++){
		ht.deleteNode(arr2[i], hfunc);
	}
	endTime = clock();
	execTime = (double)(endTime-startTime)/CLOCKS_PER_SEC;
	cout << "\t\tavg DELETE time: " << ((double)execTime/(100)) << endl;

	//record INSERT times
	startTime = clock();
	for(int i = 0; i < 100; i++){
		ht.insert(arr2[i], hfunc);
	}
	endTime = clock();
	execTime = (double)(endTime-startTime)/CLOCKS_PER_SEC;
	cout << "\t\tavg INSERT time: " << (double)execTime/100 << endl;

	//record LOOKUP times
	startTime = clock();
	for(int i = 0; i < 100; i++){
		ht.search(arr2[i], hfunc);
	}
	endTime = clock();
	execTime = (double)(endTime-startTime)/CLOCKS_PER_SEC;
	cout << "\t\tavg LOOKUP time: " << ((double)execTime/(100)) << endl;

  return 0;
}
