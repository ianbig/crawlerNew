//
// Created by Ian Liu on 2020/4/7.
//

#ifndef CRAWLER_MULTI_THREAD_HASHTABLE_H
#define CRAWLER_MULTI_THREAD_HASHTABLE_H

#include <iostream>
#include <vector>
#include "md5/src/md5.h"
#define MAXTABLE 1000

#include <fstream>
#include <filesystem>


namespace fs = std::filesystem;

class HashTable {
    off_t hash33(const std::string md5);
    size_t tableSize;
    struct Node {
        char *md5;
        char *url;
        Node() {
            md5 = NULL;
            url = NULL;
        }
        Node(const Node &tmp) {
            size_t urlLen = strlen(tmp.url);
            size_t md5Len = strlen(tmp.md5);
            url = new char[urlLen];
            md5 = new char[md5Len];
            memmove(url, tmp.url, urlLen);
            memmove(md5, tmp.md5, md5Len);
        }
        ~Node() {
            if(url) delete [] url;
            if(md5) delete [] md5;
        }
    };
   std::vector<struct Node> table[MAXTABLE];
   int hashIndex[MAXTABLE];
   unsigned int offet;
public:
    HashTable();
    //~HashTable();
    int contain(std::string searchMD5);
    void insert(std::string insertMD5);
    void writeDB(std::string pathDB);
    //size_t size();
};

#endif //CRAWLER_MULTI_THREAD_HASHTABLE_H
