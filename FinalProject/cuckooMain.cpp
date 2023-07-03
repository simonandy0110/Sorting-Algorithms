#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
using namespace std;

class HashTable {
private:
  int start_size;
	int table_size;
	int* table1;
  int* table2;
	int hash(int k, int tableid);
  bool rehash(int key);
  int insertHelper(int key, int tableid, int first, int c);
  int* getTable(int tableid);
public:
	HashTable();
	int insert(int key);
  int search(int key);
  int deleteNode(int key);
  void printToCSV(string inputFile, float lf, int tableid);
};

HashTable::HashTable() {
	table_size = 10009;
  start_size = 10009; //start size maintained to be able to access
    //hash functions used before rehashing
	table1 = new int[table_size];
  table2 = new int[table_size];
  //initialize both tables to have -1 at empty indeces
	for (int i=0; i<table_size; i++){
    table1[i] = -1;
    table2[i] = -1;
  }
}

int HashTable::hash(int k, int tableid) {
  if(tableid == 1){
	  return (int)(k % this->table_size);
  }
  return ((int)floor(k/this->table_size) % this->table_size);
}

bool isPrime(int a){ //checks to see if int a is a prime number
  int c = 0;
  for(int i = 2; i <= a/2; i++){
    if(a % i == 0) return false;
  }
  return true;
}

int nextPrime(int n){ //finds the next highest prime number following int n
  for(int i = n+1; i <= 2*n; i++){
    if(isPrime(i)) return i;
  }
  return -1;
}

bool HashTable::rehash(int key){
  int oldSize = table_size;
  //update table size to next highest prime number
  (this->table_size) = nextPrime(this->table_size);
  //copy data to new tables of new size
  int* newTable1 = new int[table_size];
  int* newTable2 = new int[table_size];
  for (int i=0; i<table_size; i++){
    if(i >= oldSize){
      //indeces present in new tables but not in old tables are empty
      newTable1[i] = -1;
      newTable2[i] = -1;
    }else{
      //copy over old data
      newTable1[i] = table1[i];
      newTable2[i] = table2[i];
    }
  }
  delete table1;
  delete table2;
  table1 = newTable1;
  table2 = newTable2;
  //attempt to insert problematic key again
  insert(key);
  return true;
}

//for the search function, use a for loop to check every index given by
  //every hash function that has been used by temporarily changing table size
  //back to previous sizes
int HashTable::search(int key){
  int index1, index2, size;
  //store table size so as not to lose it
  size = this->table_size;
  //use for loop to find all potentially used hash functions
  for(int i = table_size; i >= start_size; i--){
    this->table_size = i;
    index1 = hash(key,1);
    index2 = hash(key,2);
    if(table1[index1] == key){
      //reset table_size to what it was before the for loop
      this->table_size = size;
      //return found index of key
      return index1;
    }
    else if(table2[index2] == key){
      //reset table_size to what it was before the for loop
      this->table_size = size;
      //return found index of key
      return index2;
    }
  }
  //reset table_size
  this->table_size = size;
  return -1;
}

int HashTable::insert(int key){
  insertHelper(key, 1, key, 0);
  return 0;
}

int HashTable::insertHelper(int key, int tableid, int first, int c){
  if(key == first && c > 0){
    //cout << "\trehashing" << endl;
    rehash(key);
    return -1;
  }
  if(search(key) != -1){
    //key exists in table already, do not insert duplicate
    return -1;
  }
  int index = hash(key, tableid);
  //if inserting into table 1
  if(tableid == 1){
    if(table1[index] == -1){
      //index is empty, insert key
      table1[index] = key;
      return 0;
    }else{
      int temp = table1[index];
      //insert key
      table1[index] = key;
      tableid = 2;
      //displace to opposite table
      insertHelper(temp, tableid, first, c+1);
      return 0;
    }
  }
  //if inserting into table 2
  else{
    if(table2[index] == -1){
      //index is empty, insert key
      table2[index] = key;
      return 0;
    }else{
      int temp = table2[index];
      //insert key
      table2[index] = key;
      tableid = 1;
      //displace to opposite table
      insertHelper(temp, tableid, first, c+1);
      return 0;
    }
  }
  return 0;
}

int HashTable::deleteNode(int key){
  int index = search(key);
  //index not found, nothing to delete
  if(index == -1){
    return -1;
  }
  if(table1[index] == key){
    table1[index] = -1;
    return 0;
  }else if(table2[index] == key){
    table2[index] = -1;
    return 0;
  }
  return -1;
}

int main(int argc, char* argv[]){
  HashTable ht;
  ifstream fs;
  string inputFile = argv[1], x;
  float lf = stof(argv[2]); //load factor
  //cout << "f" << endl;
	int load = lf * 10000; //number of ints to load
  int arr1[load];
  int arr2[100];
  fs.open(inputFile);

  //insert appropriate number of ints from CSV file
    //also store into arr1[] for use in next part
  for(int i = 0; i < load; i++){
    getline(fs, x, ',');
    arr1[i] = stoi(x);
    ht.insert(arr1[i]);
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

  cout << "CUCKOO HASHING" << endl;
	cout << "\tLOAD FACTOR: " << lf << "\tHASH FUNCTIONS 1 & 2" << endl;

  //DELETE
  startTime = clock();
  for(int i = 0; i < 100; i++){
    ht.deleteNode(arr2[i]);
  }
  endTime = clock();
  execTime = (double)(endTime-startTime)/CLOCKS_PER_SEC;
  cout << "\t\tavg DELETE time: " << (double)(execTime/100) << endl;

  //INSERT
  startTime = clock();
  for(int i = 0; i < 100; i++){
    ht.insert(arr2[i]);
  }
  endTime = clock();
  execTime = (double)(endTime-startTime)/CLOCKS_PER_SEC;
  cout << "\t\tavg INSERT time: " << (double)(execTime/100) << endl;

  //LOOKUP
  startTime = clock();
  for(int i = 0; i < 100; i++){
    ht.search(arr2[i]);
  }
  endTime = clock();
  execTime = (double)(endTime-startTime)/CLOCKS_PER_SEC;
  cout << "\t\tavg LOOKUP time: " << (double)(execTime/100) << endl;

  return 0;
}
