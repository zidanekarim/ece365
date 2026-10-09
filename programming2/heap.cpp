#include "heap.h"


heap::heap(int capacity) {
    size = 0;
    this->capacity = capacity;
    data.resize(capacity);
}