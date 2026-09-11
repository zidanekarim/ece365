#include "hash.h"
#include <iostream>
#include <fstream> 

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Enter the filename of your dictionary" << endl;
    string dictionary;
    cin >> dictionary;

    cout << "Enter the filename of your input text" << endl;
    string input;
    cin >> input;


    cout << "Enter an output filename" << endl;
    string output;
    cin >> output;

    hashTable dict;


    return 0;
}


int processDictionary(const string &filename, hashTable table) {
    ifstream dictFile(filename);
    if (!dictFile.is_open()) {
        return -1; 
    }

    string line;
    while (getline(dictFile, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) { // std text prcoessing stuff
            continue;
        }

        for (char &c : line) {
            c = tolower(static_cast<unsigned char>(c)); // googled since I was unfamiliar with how to do in c++
        }

        // Insert into hash table (pv defaults to nullptr)
        int status = table.insert(line); // rehash everytime here, so we dont have to track size
        if (status == 2) {
            dictFile.close();
            return 2;
        }
    }

    dictFile.close();
    return 0;
}