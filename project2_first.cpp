// project2_first.cpp
// Zachary Friedland
#include "dictionary.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv) {

    string input = argv[1];
    Dictionary d = Dictionary::readFromFile(input);
    d.infoDump();
    if(d.find(";adslfj;adklsjf;asdkjf;kadsjlf")){
        cout << "hello found" << endl;
    }
    else{
        cout << "hello not found" << endl;
    }
    return 0;
}