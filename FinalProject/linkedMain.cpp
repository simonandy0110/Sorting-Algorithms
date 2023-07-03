#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
using namespace std;

struct LLNode{
  int key;
  LLNode *next;
  LLNode *prev;
};

class HashTable {
private:
	int TABLE_SIZE;
	LLNode* *table;
	int hash1(int k);
  int hash2(int k);
  LLNode* createNode(int key, LLNode* next);
public:
	HashTable();
	bool insert(int key, int h);
  LLNode* search(int key, int h);
  bool deleteNode(int key, int h);
};

HashTable::HashTable() {
	TABLE_SIZE = 10009;
	table = new LLNode*[TABLE_SIZE];
	for (int i=0; i<TABLE_SIZE; i++)
		table[i] = nullptr;
}

int HashTable::hash1(int k) { // h(x) function
	return k % TABLE_SIZE;
}

int HashTable::hash2(int k) { // h'(x) function
	return (int)(floor(k/TABLE_SIZE)) % TABLE_SIZE;
}

LLNode* HashTable::createNode(int key, LLNode* next)
{
  //creates and returns a new node with key, inserted before node next
  LLNode* newNode = new LLNode;
  newNode->key = key;
  newNode->next = next;
  return newNode;
}

bool HashTable::insert(int key, int h){
  //avoid duplicates, only insert if not found
  if(!search(key, h))
  {
    int index; //get index from passed hash function H
    if(h == 1){
      index = hash1(key);
    }else{
      index = hash2(key);
    }
    //insert new node with key at head of linked list
    LLNode* newNode = createNode(key, table[index]);
    table[index] = newNode;
    return true;
   }
  else{
    //duplicate entry, nothing inserted
    return false;
  }
}

LLNode* HashTable::search(int key, int h)
{
  int index; //get index from passed hash function H
  if(h == 1){
    index = hash1(key);
  }else{
    index = hash2(key);
  }
  LLNode* temp = table[index];
  if (temp == nullptr){
    //index is empty
    return nullptr;
  }
  //cycle through linked list at index
  while(temp != nullptr){
    if(temp->key == key){
      //key found at index
      return temp;
    }
    temp = temp->next;
  }
  //key not found at index
  return nullptr;
}

bool HashTable::deleteNode(int key, int h){
  int index; //get index from passed hash function H
  if(h == 1){
    index = hash1(key);
  }else{
    index = hash2(key);
  }
  LLNode* n = search(key, h);
  if(n == nullptr){
    //key not found, nothing to delete
    return 0;
  }else{
    //delete node
    //find prev node
    int ndx = 0; // index of key within linked list
    LLNode* temp = table[index];
    LLNode* prev = nullptr;
    while(temp->key != key && temp != nullptr){
      ndx++;
      prev = temp;
      temp = temp->next;
    }
    //DELETING HEAD CASE
    if(ndx == 0 || prev == nullptr){
      table[index] = n->next;
      delete n;
      return 0;
    }
    //ELSE
    prev->next = n->next;
    delete n;
    return 0;
  }
  return -1;
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
  cout << "LINKED CHAINING" << endl;
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
