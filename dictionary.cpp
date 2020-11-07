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

Dictionary::Dictionary(int tsize, Hash24 h){
    tempTable = new int[tsize];
    table.resize(tsize);
    primaryHash = h;
}

Dictionary::~Dictionary() {
    delete[] tempTable;
}

void Dictionary::writeToFile(string fName){
    fstream f;
    f.open(fName, ios::out);
    const Hash24 primHash = primaryHash;
    f.write((char*)(&primHash), sizeof(primHash));
    int primTableSize = table.size();
    f.write((char*)(&primTableSize), sizeof(primTableSize));
    for(int i = 0; i < table.size(); i++){
        int nodeSize = table[i].getTable2().size();
        f.write((char*)(&nodeSize), sizeof(nodeSize));
        for(string word : table[i].getTable2()){
            int stringSize = sizeof(word);
            f.write((char*)(&stringSize), sizeof(stringSize));
            f.write(word.c_str(), stringSize);
        }
        const Hash24 h = table[i].getH2();
        f.write((char*)(&h), sizeof(h));
    }
    f.close();
}

Dictionary Dictionary::readFromFile(string fname){
    fstream f;
    f.open(fname, ios::in);
    int primTableSize;
    int table2Size;
    Hash24 primHash;
    Hash24 hash;
    int wordLength;
    vector<Node> tempTable;
    vector<string> stringtemp;
    f.read((char*)(&primHash), sizeof(primHash));
    f.read((char*)(&primTableSize), sizeof(primTableSize));
    Dictionary d = Dictionary(primTableSize, primHash);
    d.createSecondaryHashTable();
    for(int i = 0; i < primTableSize; i++){
        f.read((char*)(&table2Size), sizeof(table2Size));
        for(int j = 0; j < table2Size; j++){
            f.read((char*)(&wordLength), sizeof(wordLength));
            char cString[wordLength];
            f.read((char*)(&cString), wordLength);
            string word(cString);
            d.pushBackTable2(word, i);
        }
        f.read((char*)(&hash), sizeof(hash));
        d.setHash2(hash, i);
    }
    f.close();
    return d;
}

Dictionary Dictionary::readFromTextFile(string fName){
    int count = 0;
    ifstream input(fName);
    
    string line;
    if(input.is_open()) {
        while(getline(input, line)){
            count++;
        }
        input.close();
    }
    return Dictionary(fName, count);
}



// int* Dictionary::getTempTable() {
//     return tempTable;
// }

void Dictionary::infoDump() {
    primaryHash.dump();
    cout << "Number of words = " << numWords << endl;
    cout << "Table size = " << numWords << endl;
    this->checkPrimaryCollisions();
    this->createSecondaryHashTable();
    this->insertWords();
    this->checkSecondaryCollisions();
}

void Dictionary::setTempTable(){
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
}

void Dictionary::checkPrimaryCollisions(){
    this->setTempTable();
    // find max collisons
    int maxCollisions = 0;
    int maxIndex = -1;
    for(int k = 0; k < numWords; k++){
        if(tempTable[k] > maxCollisions){
            maxCollisions = tempTable[k];
            maxIndex = k;
        }
    }
    cout << "Max collisions = " << maxCollisions - 1  << endl;

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
        string word = wordList[i];
        int targetIndex = primaryHash.hash(word) % numWords;
        int table2Target = table[targetIndex].getH2().hash(word) % (table[targetIndex].getTable2().size());
        if(table[targetIndex].getTable2()[table2Target].empty()){
            table[targetIndex].getTable2()[table2Target] = word;
        }
        else{
            collisionHelper(targetIndex, i);
        }
    }
}

void Dictionary::collisionHelper(int primaryHashIndex, int currentIndex) {
    table[primaryHashIndex].getTable2().clear();
    int nSquared = tempTable[primaryHashIndex] * tempTable[primaryHashIndex];
    table[primaryHashIndex].getTable2().resize(nSquared);
    table[primaryHashIndex].newHash();
    for(int i = 0; i <= currentIndex; i++){
        string word = wordList[i];
        int targetIndex = primaryHash.hash(word) % numWords;
        int table2Target = table[targetIndex].getH2().hash(word) % (table[targetIndex].getTable2().size());
        if(targetIndex == primaryHashIndex){
            if(table[primaryHashIndex].getTable2()[table2Target].empty()){
                table[primaryHashIndex].getTable2()[table2Target] = word;
            }
            else{
                collisionHelper(primaryHashIndex, currentIndex);
            }
        }
    }
}

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
    int targetIndex = primaryHash.hash(word) % table.size();
    if(table[targetIndex].getTable2().size() < 1) {
        cout << word << " not found" << endl;
        return false;
    }
    int table2Target = table[targetIndex].getH2().hash(word) % (table[targetIndex].getTable2().size());
    if(table[targetIndex].getTable2()[table2Target] == word){
        cout << word << " found" << endl;
        return true;
    }
    cout << word << " not found" << endl;
    return false;
}

// void Dictionary::createTable(){
//     this->setTempTable();
//     this->createSecondaryHashTable();
//     this->insertWords();
// }

// void Dictionary::setTable2(vector<string> stringVector, Hash24 h, int index){
//     setTable2Helper(stringVector, h, index);
// }

// void Dictionary::setTable2Helper(vector<string> stringVector, Hash24 h, int index){
//     Node n = Node(stringVector.size());
//     for(int i = 0; i < stringVector.size(); i++){
//         n.getTable2()[i] = stringVector[i];
//     }
//     n.setH2(h);
// }

void Dictionary::pushBackTable2(string word, int index){
    table[index].getTable2().push_back(word);
}

void Dictionary::setHash2(Hash24 h, int index) {
    table[index].setH2(h);
}

void Dictionary::printAll(){
    for(int j = 0; j < table.size(); j++){
        cout << "Index: " << j << " words: ";
        for(int k = 0; k < table[j].getTable2().size(); k++){
            cout << table[j].getTable2()[k] << " ";
        }
        cout << endl;
    }
}
