#include "hash.h"
#include <iostream>
#include <fstream> 
#include <chrono>
using namespace std;


int processDictionary(const string &filename, hashTable &table) {
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

int spellCheck(const string &inputFile, const string &outputFile, hashTable &table) {
    ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        return -1; 
    }
    
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        return -1; 
    }
    auto isWordChar = [](char c) -> bool { // more efficient than a separate check, less than const
        return isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '\'';
    };


    string line;
    int lineCounter = 0;
    while (getline(inFile, line)) {
        lineCounter++;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) { // std text prcoessing stuff
            continue;
        }
        string wordbuilder;
        for (int i = 0; i < line.length();) {
            char c = tolower(static_cast<unsigned char>(line[i]));

            if (isWordChar(c)) {
                wordbuilder += c;

                // length check
                if (wordbuilder.length() > 20) { 
                    outFile << "Long word at line " << lineCounter << ", starts: " << wordbuilder.substr(0, 20) << "\n";
                    
                    // skips rest of word 
                    while (i < line.length() && isWordChar(line[i])) {
                        i++;
                    }
                    
                    wordbuilder.clear();
                    continue; 
                }
                i++;
            } else {
                // actual spell check here
                if (!wordbuilder.empty()) {
                    if (!table.contains(wordbuilder)) {
                        outFile << "Unknown word at line " << lineCounter << ": " << wordbuilder << "\n";
                    }
                    wordbuilder.clear();
                }
                i++;
            }
        }

        // Check for any word that ends right at the end of the line
        if (!wordbuilder.empty()) {
            if (!table.contains(wordbuilder)) {
                outFile << "Unknown word at line " << lineCounter << ": " << wordbuilder << "\n";
            }
            wordbuilder.clear();
        }

        
    }

    return 0;
}

int main(int argc, char* argv[]) {
    cout << "Enter name of dictionary: ";
    string dictionaryName;
    cin >> dictionaryName;

    hashTable dict;
    auto start = chrono::high_resolution_clock::now();
    int processed = processDictionary(dictionaryName, dict);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end-start;
    cout << "Total time (in seconds) to load dictionary: " << duration.count() << endl;

    cout << "Enter name of input file: ";
    string input;
    cin >> input;


    cout << "Enter name of output file: ";
    string output;
    cin >> output;

    start = chrono::high_resolution_clock::now();
    processed = spellCheck(input, output, dict);
    end = chrono::high_resolution_clock::now();
    duration = end-start;

    cout << "Total time (in seconds) to check document: " << duration.count() << endl;
    return 0;
}

