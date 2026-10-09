#ifndef HEAP_H
#define HEAP_H
#include hash.h
/*
My heap class contains four private data members. 
Two are integers representing the capacity and the current size of the heap.
 The third is a vector of node objects containing the actual data of the heap; each node contains a string id, an integer key, and a pointer to void that can point to anything. (I made "node" a private nested class within the heap class.) The fourth private data member is a hash table. Since the heap constructor is provided with the maximum size of the heap, you may construct the hash table to be large enough such that there is a small likelihood of a rehash, but that is up to you (I'll discuss how to do that in class). Note that since items get removed from the heap, but only lazily deleted from the hash table, it is still possible that a rehash of the hash table will be necessary.*/
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
    hashTable mapping; // This is the hash table that maps ids to nodes.


    }
#endif