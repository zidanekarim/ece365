#include "hash.h"


int hashTable::insert(const std::string &key, void *pv) {
    if (filled >= capacity / 2) {
        if (!rehash()) {
            return 2; 
        }
    }

    if (contains(key)) { // we'll leverage findPos in contains to check for occupied/deleted
        return 1;
    }
    // now we do linear probing according to assignment specs
    int pos = hash(key) % capacity;
    if (pos < 0) pos += capacity;

    while (data[pos].isOccupied && !data[pos].isDeleted) {
        pos = (pos + 1) % capacity; 
    }

    data[pos] = hashItem{key, true, false, pv};
    filled++;
    return 0;
}

hashTable::hashTable(int size = 0) {
    capacity = getPrime(size);
    filled=0;
    data.resize(capacity);
} 

bool hashTable::contains(const std::string &key) {
    return findPos(key) != -1;
}

int hash(const std::string &key) {
    
}
bool rehash();



void *getPointer(const std::string &key, bool *b) {
    int index = findPos(key);
    if (index == -1) { // avoiding contains here to not duplicate operation
        if (b != nullptr) {
            *b = false;
        }
        return nullptr; 
    }
    if (b != nullptr) {
        *b = true;
    }
    return data[index].pv;

}


int setPointer(const std::string &key, void *pv) {
    int index = findPos(key);
    if (index == -1) { // avoiding contains here to not duplicate operation
        return 1; 
    }
    data[index].pv = pv;
    return 0;
}


bool remove(const std::string &key); // this function im not implementing right now since i dont know what it wants



static unsigned int hashTable::hashItem::getPrime(int size) {
    auto isPrime [](int n) -> bool  { // lambda function, will use trial division here
        if (n <= 1) return false;
        if (n == 3) return true;
        for (int i = 5; i * i <= n; i += 6) { // https://math.stackexchange.com/questions/616093/why-every-prime-3-is-represented-a
            if (n % i == 0 || n % (i + 2) == 0) { // prev knowledge algorithm to compute if number is prime
                return false;
            }
        }   
        return true;
    }
    
    
    int start = size;
    if (start <= 2) return 2;
    if (start % 2 == 0) {
        start += 1;
    }
    while (!isPrime(start)) {
        start += 2;
    }
    return start;
}


