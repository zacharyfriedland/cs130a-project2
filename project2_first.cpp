// project2_first.cpp
// Zachary Friedland
#include "dictionary.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv) {

    string input = argv[1];
    string input2 = argv[2];
    Dictionary d = Dictionary::readFromTextFile(input);
    d.infoDump();
    d.writeToFile(input2);
    return 0;
}
