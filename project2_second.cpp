// project2_second.cpp
// Zachary Friedland
#include "dictionary.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main(int argc, char** argv) {
    string in = argv[1];
    string argument = argv[2];
    vector<string> input;
    Dictionary d = Dictionary::readFromFile(in);
    Dictionary d2 = Dictionary::readFromTextFile("PA2_dataset_10000.txt");
    d2.createTable();
    //d2.infoDump();
    // push words in argument into vector
    string word;
    for(int i = 0; i < argument.size(); i++){
        if(argument[i] == ','){
            input.push_back(word);
            word = "";
            i++;
        }
        else if(argument[i] == ' '){
            continue;
        }
        else if(i == argument.size() - 1){
            word += argument[i];
            input.push_back(word);
        }
        else{
            word += argument[i];
        }
    }
    // find words in input vector
    for(int j = 0; j < input.size(); j++){
        //d.find(input[j]); 
        d2.find(input[j]);
    }
    
    return 0;
}
