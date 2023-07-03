#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
using namespace std;

class HashTable {
private:
	int TABLE_SIZE;
	int* table;
  int hash(int key, int h);
public:
	HashTable();
  int insert(int key, int h);
  int search(int key, int h);
  bool deleteNode(int key, int h);
};

HashTable::HashTable() {
	TABLE_SIZE = 10009;
	table = new int[TABLE_SIZE];
	for (int i=0; i<TABLE_SIZE; i++)
		table[i] = -1;
		//initialize all empty indeces to -1
}


int HashTable::hash(int key, int h) { //returns index for given key & hash function
	if (h ==1) return key % TABLE_SIZE;
	else return (int)(floor(key/TABLE_SIZE)) % TABLE_SIZE;
}

int HashTable::search(int key, int h){
  int index = hash(key, h);
  if(table[index] == key){
    return index;
  }else{
		//search following indeces for probed key
		//for loop wrapping around table from index to index-1
    for(int i = 0; i < TABLE_SIZE; i++){
			int x = (i + index) % TABLE_SIZE;
      if(table[x] == key) return x;
    }
    return -1;
  }
}

bool HashTable::deleteNode(int key, int h){
	int index = hash(key, h);
  if(table[index] == key){
		//key found at hashed index
    return index;
  }else{
		//search following indeces for probed key
		//for loop wrapping around table from index to index-1
    for(int i = 0; i < TABLE_SIZE; i++){
			int x = (i + index) % TABLE_SIZE;
      if(table[x] == key){
				//key found, deleting
				table[x] = -1;
				return 0;
			}
    }
		//key could not be found
    return -1;
  }
}

int HashTable::insert(int key, int h) {
	int index = hash(key, h);
	if(table[index] == key){
		//key already exists at hashed index, do not insert dupliczte
    return -1;
  }
	//if index is occupied, cycle thru til next empty index is found
	if(table[index] != -1){
		//for loop wrapping around table from index to index-1
		for(int i = 0; i < TABLE_SIZE; i++){
			int x = (i + index) % TABLE_SIZE;
			//if key is found at any index following original hashed index,
			//do not enter duplicate
			if(table[x] == key) return -1;
			//insert key at first available index
			if(table[x] == -1){
				table[x] = key;
				//set i to TABLE_SIZE to end loop
				i = TABLE_SIZE;
			}
		}
	}
	//if original hashed index is empty, insert key
	table[index] = key;
	return 0;
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
  cout << "LINEAR PROBING" << endl;
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
