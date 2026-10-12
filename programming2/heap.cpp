#include "heap.h"
/*
Zidane Karim (ECE365)
Creates priority queue using a binary heap
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
    if (mapping.contains(id)) {
        return 2;
    }

    // Insert as a new leaf at the end and percolate up
    size++;
    data[size].id = id;
    data[size].key = key;
    data[size].pv = pv;

    // Insert into mapping pointing to the node slot
    mapping.insert(id, &data[size]);

    percolateUp(size);
    return 0;
}

// setKey - Updates the key of the specified node and restores heap order
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

// deleteMin - Removes and returns the item with the smallest key
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
        // ppData represents the destination address where the void* should be copied
        *(static_cast<void **>(ppData)) = data[1].pv;
    }

    // Remove minimum entry from hash table
    mapping.remove(data[1].id);

    if (size > 1) {
        data[1] = data[size];
        mapping.setPointer(data[1].id, &data[1]);
        size--;
        percolateDown(1);
    } else {
        size--;
    }

    return 0;
}

// remove - Deletes the node with the specified id from the binary heap
// Returns:
//   0 on success
//   1 if a node with the given id does not exist
int heap::remove(const std::string &id, int *pKey, void *ppData) {
    bool b = false;
    node *pn = static_cast<node *>(mapping.getPointer(id, &b));
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

        // If the moved node has smaller key, bubble up; otherwise push down
        if (temp.key < pn->key) {
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

// percolateUp - Moves the element at posCur up to restore the min-heap property
void heap::percolateUp(int posCur) {
    node temp = data[posCur];

    while (posCur > 1 && temp.key < data[posCur / 2].key) {
        data[posCur] = data[posCur / 2];
        mapping.setPointer(data[posCur].id, &data[posCur]);
        posCur /= 2;
    }

    data[posCur] = temp;
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

// percolateDown - Moves the element at posCur down to restore the min-heap property
void heap::percolateDown(int posCur) {
    node temp = data[posCur];
    int child;

    while (posCur * 2 <= size) {
        child = posCur * 2;
        // Select the smaller child
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

// getPos - Computes the index in data using pointer arithmetic
int heap::getPos(node *pn) {
    return static_cast<int>(pn - &data[0]);
}
