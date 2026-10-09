#ifndef HEAP_H
#define HEAP_H
#include "hash.h"
#include <vector>
#include <string>

class heap {
    public:
    heap(int capacity = 0);

    int insert(const std::string &id, int key, void *pv = nullptr);
    int setKey(const std::string &id, int key);
    int deleteMin(std::string *pId = nullptr, int *pKey = nullptr, void *ppData = nullptr);
    int remove(const std::string &id, int *pKey = nullptr, void *ppData = nullptr);

    private: 
        class node {
        public:
            std::string id;
            int key;
            void *pv;

            node() = default; 
        };

    int capacity;
    int size; 
    std::vector<node> data; 
    hashTable mapping; 
    
    void percolateUp(int posCur);
    void percolateDown(int posCur);
    int getPos(node *pn);


    }
#endif