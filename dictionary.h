// dictionary.h
// Zachary Friedland

#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "hash24.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// Helper class for creating a hashtable
class Dictionary {
    public:
        Dictionary(string fname, int tsize);
        ~Dictionary();
        bool find(string word);
        void writeToFile(string fName);
        static Dictionary readFromFile(string fName);
        int* getTempTable();
        void infoDump();
        void collisionHelper(int primaryHashIndex, int currentIndex);

    private:
        // in each slot an object called Node
        struct Node {
                // constructor for Node in each spot of table
                Node(){
                    hashCount = 0;
                    h2 = Hash24();
                }
                Node(int n){
                    hashCount = 1;
                    int tSize = n * n;
                    table2.resize(tSize);
                }
                void newHash() {
                    hashCount++;
                    h2 = Hash24();
                }
                int getHashCount(){return hashCount;}
                vector<string> &getTable2(){return table2;}
                Hash24 getH2() {return h2;}
                int hashCount;
                void printElements(){
                    cout << "hashCount: " << hashCount << " table2.size(): " << table2.size() << endl;
                }

            private:
                vector<string> table2;
                Hash24 h2;
        };
        vector<Node> table;
        int* tempTable;         // for original count of collisons
        void createTable(string fName);
        void insertWords();
        bool secondHashInsert(string word);
        void createSecondaryHashTable();
        Hash24 primaryHash;
        int numWords;
        int tableSize;
        string filename;
        string fileContent;
        vector<string> wordList;
        void checkPrimaryCollisions();
        void checkSecondaryCollisions();
};


#endif

