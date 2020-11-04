// dictionary.cpp
// Zachary Friedland

#include "dictionary.h"
#include "hash24.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


Dictionary::Dictionary(string fname, int tsize) {
    primaryHash = Hash24();
    numWords = tsize;
    filename = fname;
    
    ifstream input(fname);
    
    string line;
    if(input.is_open()) {
        while(getline(input, line)){
            fileContent += line + "\n";
            wordList.push_back(line);
        }
        input.close();
    }

    tempTable = new int[tsize];
    table.resize(tsize);
}

Dictionary::~Dictionary() {
    delete[] tempTable;
}

void Dictionary::writeToFile(string fName){

}

Dictionary Dictionary::readFromFile(string fName){
    int count = 0;
    ifstream input(fName);
    
    string line;
    if(input.is_open()) {
        while(getline(input, line)){
            count++;
        }
        input.close();

        // std::string content( (std::istreambuf_iterator<char>(input) ),
        //                (std::istreambuf_iterator<char>()    ) );
        
    }
    // TO DO?? Below
    return Dictionary(fName, count);
}

// void Dictionary::createTable(string fName) {
//     fstream input(fName); 
//     string line;
//     if(input.is_open()) {
//         int i = 0;
//         while(getline(input, line)){
//             Hash24 h;
//             i++;
//         }
//         input.close();
//     }
// }



int* Dictionary::getTempTable() {
    return tempTable;
}

void Dictionary::infoDump() {
    primaryHash.dump();
    cout << "Number of words = " << numWords << endl;
    cout << "Table size = " << tableSize << endl;
    this->checkPrimaryCollisions();
    this->createSecondaryHashTable();
    this->insertWords();
    this->checkSecondaryCollisions();
}

void Dictionary::checkPrimaryCollisions(){
    // initialize all indexes to 0 to prep for count
    for(int i = 0; i < numWords; i++){
        tempTable[i] = 0;
    }
    // count the number of collisions in each slot
    for(int j = 0; j < numWords; j++){
        int targetIndex = primaryHash.hash(wordList[j]) % numWords;
        if(targetIndex > numWords){
            cout << "ERROR: hash id is bigger than number of words" << endl;
        }
        tempTable[targetIndex]++;
    }
    // find max collisons
    int maxCollisions = 0;
    int maxIndex = -1;
    for(int k = 0; k < numWords; k++){
        if(tempTable[k] > maxCollisions){
            maxCollisions = tempTable[k];
            maxIndex = k;
        }
    }
    cout << "Max collisions = " << maxCollisions << endl;

    // checking primary slots with x collisions
    int wordsInSlot = 0;
    while(wordsInSlot <= 20){
        int count = 0;
        for(int a = 0; a < numWords; a++){
            if(wordsInSlot == tempTable[a]){
                count++;
            }
        }
        cout << "# of primary slots with " << wordsInSlot << " words = " << count << endl;
        tableSize += count;
        wordsInSlot++;
    }

    // printing words in slot with most collisions
    cout << "** Words in the slot with most collisions ***" << endl;
    for(int b = 0; b < numWords; b++){
        int targetIndex = primaryHash.hash(wordList[b]) % numWords;
        if(maxIndex == targetIndex){
            cout << wordList[b] << endl;
        }
    }
}

void Dictionary::createSecondaryHashTable() {
    for(int i = 0; i < table.size(); i++){
        table[i] = Node(tempTable[i]);
    }
}

void Dictionary::insertWords(){
    for(int i = 0; i < table.size(); i++){
        // cout << "inside for loop, Index: " << i << endl;
        // cout << "table size: " << table.size() << endl;
        string word = wordList[i];
        int targetIndex = primaryHash.hash(word) % numWords;
        int table2Target = table[targetIndex].getH2().hash(word) % (table[targetIndex].getTable2().size());
        if(table[targetIndex].getTable2()[table2Target].empty()){
            table[targetIndex].getTable2()[table2Target] = word;
        }
        else{
            // cout << "hit collision helper" << endl;
            collisionHelper(targetIndex, i);
        }
    }

    // for(int j = 0; j < table.size(); j++){
    //     // if(table)
    //     cout << "Index: " << j << " words: ";
    //     for(int k = 0; k < table[j].getTable2().size(); k++){
    //         cout << table[j].getTable2()[k] << " ";
    //     }
    //     cout << endl;
    // }
}

void Dictionary::collisionHelper(int primaryHashIndex, int currentIndex) {
    table[primaryHashIndex].getTable2().clear();
    int nSquared = tempTable[primaryHashIndex] * tempTable[primaryHashIndex];
    table[primaryHashIndex].getTable2().resize(nSquared);
    table[primaryHashIndex].newHash();
    // cout << "called collisionHelper" << endl;
    for(int i = 0; i <= currentIndex; i++){
        string word = wordList[i];
        int targetIndex = primaryHash.hash(word) % numWords;
        int table2Target = table[targetIndex].getH2().hash(word) % (table[targetIndex].getTable2().size());
        if(targetIndex == primaryHashIndex){
            if(table[primaryHashIndex].getTable2()[table2Target].empty()){
                table[primaryHashIndex].getTable2()[table2Target] = word;
            }
            else{
                // cout << "hitting recursive collisionHelper within collision helper" << endl;
                collisionHelper(primaryHashIndex, currentIndex);
            }
        }
    }
}

// bool Dictionary::secondHashInsert(string word) {
//     int targetIndex = primaryHash.hash(word) % numWords;
//     table[targetIndex].newHash();
//     table[targetIndex].getTable2().clear();
//     int nSquared = tempTable[targetIndex] * tempTable[targetIndex];
//     table[targetIndex].getTable2().resize(nSquared);
//     if(targetIndex > numWords){
//         cout << "ERROR: hash id is bigger than number of words" << endl;
//     }
//     int table2Target = table[targetIndex].getH2().hash(word) % (table[targetIndex].getTable2().size());
//     if(table[targetIndex].getTable2()[table2Target].empty()){
//         table[targetIndex].getTable2()[table2Target] = word;
//         return true;
//     }
//     return false;
// }


void Dictionary::checkSecondaryCollisions() {
    double avgHash = 1;
    int total = 0;
    int hashFunctionCount = 1;
    while(hashFunctionCount <= 20){
        int count = 0;
        for(int i = 0; i < table.size(); i++){
            if(hashFunctionCount == table[i].getHashCount()){
                count++;
            }
        }
        cout << "# of secondary hash tables trying " << hashFunctionCount << " hash functions = " << count << endl;
        if(count > 0){
            avgHash += hashFunctionCount * count;
        }
        total += count;
        hashFunctionCount++;
    }
    avgHash = (double)(avgHash/total);
    cout << "Average # of hash functions tried = " << avgHash << endl;
}

bool Dictionary::find(string word) {
    int targetIndex = primaryHash.hash(word) % numWords;
    int table2Target = table[targetIndex].getH2().hash(word) % (table[targetIndex].getTable2().size());
    if(table[targetIndex].getTable2()[table2Target] == word){
        return true;
    }
    return false;
}


    
// main.cpp

// Dictionary d = Dictionary::readFromFile('blah.txt');
// d.createTable


