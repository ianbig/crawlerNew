//
// Created by Ian Liu on 2020/4/7.
//

#include "HashTable.h"

int HashTable::contain(std::string searchMD5) {
    size_t index = hash33(searchMD5);

    for(std::vector<Node>::iterator iter = table[index].begin(); iter != table[index].end(); iter++) {
        if(iter->md5 == searchMD5)  {
            std::cout << iter->md5 << std::endl;
            return index;
        }
    }

    return -1;
}

bool HashTable::insert(std::string inseartMD5) {
    size_t index = contain(inseartMD5);
    struct Node newElement;
    newElement.md5 = inseartMD5;

    if(index == -1 ) index = hash33(inseartMD5);
    table[index].push_back(newElement);

}

off_t HashTable::hash33(const std::string str) {
    size_t hashValue = 0;
    int coff = 33;

    for( int i = 0; i < str.size(); i++ ) {
        hashValue = hashValue * coff + str[i];
    }

    return hashValue % MAXTABLE;
}