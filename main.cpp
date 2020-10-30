// main.cpp
// Zachary Friedland
#include "dictionary.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv) {

    string input = argv[1];
    Dictionary d = Dictionary::readFromFile(input);
    d.infoDump();
    return 0;
}