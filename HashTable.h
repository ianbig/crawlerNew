//
// Created by Ian Liu on 2020/4/7.
//

#ifndef CRAWLER_MULTI_THREAD_HASHTABLE_H
#define CRAWLER_MULTI_THREAD_HASHTABLE_H

#include <iostream>
#include <vector>
#include "md5/src/md5.h"
#define MAXTABLE 1000

class HashTable {
    off_t hash33(const std::string md5);
    size_t tableSize;
    struct Node {
        std::string md5;
    };
   std::vector<struct Node> table[MAXTABLE];
public:
    //HashTable();
    //~HashTable();
    int contain(std::string searchMD5);
    bool insert(std::string insertMD5);
    //size_t size();
};

#endif //CRAWLER_MULTI_THREAD_HASHTABLE_H
