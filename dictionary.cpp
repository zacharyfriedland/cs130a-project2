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
    table = new string[tsize];
}

Dictionary::~Dictionary() {
    delete[] tempTable;
    delete[] table;
}

bool Dictionary::find(string word)
{
    return false;
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

string* Dictionary::getTable() {
    return table;
}

int* Dictionary::getTempTable() {
    return tempTable;
}

void Dictionary::infoDump() {
    primaryHash.dump();
    cout << "Number of words = " << numWords << endl;
    // TO DO below table size
    cout << "Table size = " << tableSize << endl;
    this->checkPrimaryCollisions();
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

    
// main.cpp

// Dictionary d = Dictionary::readFromFile('blah.txt');
// d.createTable


