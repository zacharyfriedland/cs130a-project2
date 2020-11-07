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
        Dictionary(){ }
        Dictionary(string fname, int tsize);
        Dictionary(int tsize, Hash24 h);
        ~Dictionary();
        bool find(string word);
        void writeToFile(string fName);
        static Dictionary readFromFile(string fname);
        static Dictionary readFromTextFile(string fName);
        void setTempTable();
        int* getTempTable();
        void infoDump();
        void collisionHelper(int primaryHashIndex, int currentIndex);
        void createTable();
        void createSecondaryHashTable();
        void setTable2(vector<string> stringVector, Hash24 h, int index);
        void setPrimHash(Hash24 h){primaryHash = h;}
        void pushBackTable2(string word, int index);
        void setHash2(Hash24 h, int index);
        void printAll();


    private:
        // in each slot an object called Node
        struct Node {
                // constructor for Node in each spot of table
                Node(){
                    hashCount = 0;
                    h2 = Hash24();
                }
                Node(int n){
                    hashCount = 0;
                    int tSize = n * n;
                    table2.resize(tSize);
                }
                Node(vector<string> v, Hash24 h){
                    hashCount = 1;
                    table2.resize(v.size());
                    for(int i = 0; i < v.size(); i++){
                        table2[i] = v[i];
                    }
                    h2 = h;
                }
                void newHash() {
                    hashCount++;
                    h2 = Hash24();
                }
                int getHashCount(){return hashCount;}
                vector<string> &getTable2(){return table2;}
                Hash24 getH2() {return h2;}
                void setH2(Hash24 h){h2 = h;}
                void printElements(){
                    cout << "hashCount: " << hashCount << " table2.size(): " << table2.size() << endl;
                }

            private:
                vector<string> table2;
                Hash24 h2;
                int hashCount;
        };
        vector<Node> table;
        int* tempTable;         // for original count of collisons
        void insertWords();
        bool secondHashInsert(string word);
        Hash24 primaryHash;
        int numWords;
        int tableSize;
        string filename;
        string fileContent;
        vector<string> wordList;
        void checkPrimaryCollisions();
        void checkSecondaryCollisions();
        void setTable2Helper(vector<string> stringVector, Hash24 h, int index);
};


#endif

