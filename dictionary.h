// dictionary.h
// Zachary Friedland

#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "hash24.h"
#include <string>
#include <vector>

// Helper class for creating a hashtable
class Dictionary {
    public:
        Dictionary(std::string fname, int tsize);
        ~Dictionary();
        bool find(std::string word);
        void writeToFile(std::string fName);
        static Dictionary readFromFile(std::string fName);
        std::string* getTable();
        int* getTempTable();
        void infoDump();

    private:
        std::string* table;
        int* tempTable;         // for original count of collisons
        void createTable(std::string fName);
        Hash24 primaryHash;
        int numWords;
        int tableSize;
        std::string filename;
        std::string fileContent;
        std::vector<std::string> wordList;
        void checkPrimaryCollisions();
};

#endif

