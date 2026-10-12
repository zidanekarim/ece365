#include "heap.h"
/*
Zidane Karim (ECE365)
Creates priority queue using a binary heap

note: the function signatures contain comments that are taken from the assignments example; I thought although this was NOT my work, they sufficiently described the 
functions and there wasn't a point in arbitrarily rewriting such comments. 
*/

heap::heap(int capacity) {
    size = 0;
    this->capacity = capacity;
    data.resize(capacity);
}

// insert - Inserts a new node into the binary heap
// Returns:
//   0 on success
//   1 if the heap is already filled to capacity
//   2 if a node with the given id already exists (but not full)
int heap::insert(const std::string &id, int key, void *pv) {
    if (size >= capacity) {
        return 1;
    }
    if (mapping.contains(id)) { // hashtable contains
        return 2;
    }

  
    size++;
    data[size].id = id;
    data[size].key = key;
    data[size].pv = pv;

    mapping.insert(id, &data[size]);

    percolateUp(size);
    return 0;
}

// setKey - set key of node to value
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
int heap::setKey(const std::string &id, int key) {
    bool b = false;
    node *pn = static_cast<node *>(mapping.getPointer(id, &b));
    if (!b || pn == nullptr) {
        return 1;
    }

    int oldKey = pn->key;
    pn->key = key;
    int pos = getPos(pn);

    if (key < oldKey) {
        percolateUp(pos);
    } else if (key > oldKey) {
        percolateDown(pos);
    }

    return 0;
}

// deleteMin - Removes and sets ppdata to the item with the smallest key
// Returns:
//   0 on success
//   1 if the heap is empty
int heap::deleteMin(std::string *pId, int *pKey, void *ppData) {
    if (size == 0) {
        return 1;
    }

    if (pId != nullptr) {
        *pId = data[1].id;
    }
    if (pKey != nullptr) {
        *pKey = data[1].key;
    }
    if (ppData != nullptr) {
        *(static_cast<void **>(ppData)) = data[1].pv;
    }

    // removes minimum entry
    mapping.remove(data[1].id);

    if (size > 1) {
        data[1] = data[size];
        mapping.setPointer(data[1].id, &data[1]);
        size--;
        percolateDown(1);
    } else {
        size--; // we dont need to percolate down if all items gone
    }

    return 0;
}

// remove - Deletes node with specified id
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
int heap::remove(const std::string &id, int *pKey, void *ppData) {
    bool b = false;
    node *pn = static_cast<node *>(mapping.getPointer(id, &b)); // get pointer to node with id
    if (!b || pn == nullptr) {  
        return 1;
    }

    if (pKey != nullptr) {
        *pKey = pn->key; 
    }
    if (ppData != nullptr) {
        *(static_cast<void **>(ppData)) = pn->pv; 
    }

    int pos = getPos(pn);
    mapping.remove(id);

    if (pos == size) {
        size--;
    } else {
        node temp = data[size];
        size--;


        if (temp.key < pn->key) { // if the new key is smaller than the old key, percolate up because it may be smaller than its parent. 
            // otherwise, percolate down because it may be larger than its children.
            data[pos] = temp;
            mapping.setPointer(data[pos].id, &data[pos]);
            percolateUp(pos);
        } else {
            data[pos] = temp;
            mapping.setPointer(data[pos].id, &data[pos]);
            percolateDown(pos);
        }
    }

    return 0;
}

// percolateUp - Moves the element at posCur up to restore the min-heap 
void heap::percolateUp(int posCur) {
    node temp = data[posCur];

    while (posCur > 1 && temp.key < data[posCur / 2].key) {
        data[posCur] = data[posCur / 2]; // move parent down
        mapping.setPointer(data[posCur].id, &data[posCur]); // update pointer in hash table
        posCur /= 2; // move up to parent position
    }

    data[posCur] = temp; 
    mapping.setPointer(data[posCur].id, &data[posCur]); // setpointer implemented in assignment 1
}

// percolateDown - Moves the element at posCur down to restore the min-heap 
void heap::percolateDown(int posCur) {
    node temp = data[posCur];
    int child;

    while (posCur * 2 <= size) {
        child = posCur * 2;
        // pretty much same as percolate up, but we check the children instead of the parent.
        if (child < size && data[child + 1].key < data[child].key) {
            child++;
        }

        if (data[child].key < temp.key) {
            data[posCur] = data[child];
            mapping.setPointer(data[posCur].id, &data[posCur]);
            posCur = child;
        } else {
            break;
        }
    }

    data[posCur] = temp;
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

// getPos - gets index
int heap::getPos(node *pn) {
    return static_cast<int>(pn - &data[0]);
}
