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

    data[pos].key = key;
    data[pos].isOccupied = true;
    data[pos].isDeleted = false;
    data[pos].pv = pv;
    filled++;
    return 0;
}

hashTable::hashTable(int size) {
    capacity = getPrime(size);
    filled=0;
    data.resize(capacity);
} 

bool hashTable::contains(const std::string &key) { // redundant within class but useful as public
    return hashTable::findPos(key) != -1;
}

int hashTable::hash(const std::string &key) {
    // DJB2 algorithm, which I sourced from the internet
    unsigned int hash_val = 5381;
    for (char c : key) {
        hash_val = (hash_val * 33) + c;
    }

    return hash_val % capacity;
}
bool hashTable::rehash() {
    int nextPrime = getPrime(capacity  * 2);
    if (nextPrime <= capacity) return false;
    std::vector<hashItem> tempData = data;
    capacity = nextPrime;
    try {
        capacity = nextPrime;
        data.clear(); 
        data.resize(capacity);
    } catch (const std::bad_alloc &) {
        // restoring state
        data = tempData;
        capacity = tempData.size();
        return false;
    }
    
    filled = 0;

    for (const auto &item : tempData) { // from Google, reference saves memory footprint here 
        if (item.isOccupied && !item.isDeleted) { // again not necessary here but future-proofing
            insert(item.key, item.pv); // insert increases filled count
        }
    }
    
    return true;
}



void* hashTable::getPointer(const std::string &key, bool *b) {
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


int hashTable::setPointer(const std::string &key, void *pv) {
    int index = findPos(key);
    if (index == -1) { // avoiding contains here to not duplicate operation
        return 1; 
    }
    data[index].pv = pv;
    return 0;
}


//bool hashTable::remove(const std::string &key); // this function im not implementing right now since i dont know what it wants



unsigned int hashTable::getPrime(int size) {
    auto isPrime = [](int n) -> bool  { // lambda function, will use trial division here
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;

        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    };
    
    
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


int hashTable::findPos(const std::string &key) {
    int pos = hash(key) % capacity; // according to hash algo defined in hash()
    if (pos < 0) pos += capacity;

    while (data[pos].isOccupied) { 
        if (!data[pos].isDeleted && data[pos].key == key) {
            return pos;
        }
        pos = (pos + 1) % capacity;
    }

    return -1;
}