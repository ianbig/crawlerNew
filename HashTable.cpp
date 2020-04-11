//
// Created by Ian Liu on 2020/4/7.
//

#include "HashTable.h"

HashTable::HashTable() {
    memset(hashIndex, -1, MAXTABLE);
    offet = 0;
}

int HashTable::contain(std::string searchURL) {
    std::string searchMD5 = MD5(searchURL).toStr();
    size_t index = hash33(searchMD5);

    if(table[index].empty()) return -2;

    for(std::vector<Node>::iterator iter = table[index].begin(); iter != table[index].end(); iter++) {
        if(iter->md5 == searchMD5)  {
            return index;
        }
    }

    return -1;
}

void HashTable::insert(std::string insertURL) {
    std::string insertMD5 = MD5(insertURL).toStr();
    int index = contain(insertMD5);
    struct Node newElement;
    newElement.md5 = new char [insertMD5.length() + 1];
    newElement.url = new char [insertURL.length() + 1];
    memmove(newElement.md5, insertMD5.c_str(), insertMD5.length());
    newElement.md5[insertMD5.length()] = 0;
    memmove(newElement.url, insertURL.c_str(), insertURL.length());
    newElement.url[insertURL.length()] = 0;

    if(index == -1 || index == -2) {
        if(index == -2)
            hashIndex[offet++] = (index = hash33(insertMD5) );

    }
    table[index].push_back(newElement);
    tableSize++;

}

off_t HashTable::hash33(const std::string str) {
    size_t hashValue = 0;
    int coff = 33;

    for( int i = 0; i < str.size(); i++ ) {
        hashValue = hashValue * coff + str[i];
    }

    return hashValue % MAXTABLE;
}

void HashTable::writeDB(std::string pathDB) {
    // check Hashtable DB status
    fs::path path(pathDB);
    int index = 1;
    int offIndex = 0;
    int tmp = 0;

    if(!fs::exists(path)) {
        fs::create_directory(path);
    }

    while(fs::exists(path.string() + "/hash" + std::to_string(index) + ".log")) {
        index++;
    }
    path = path.string() + "/hash" + std::to_string(index) + ".log";
    std::ofstream fptr;
    std::string a = path.string();
    fptr.open(path.string(), std::ios::out);
    while(hashIndex[offIndex] != -1) {
        tmp = hashIndex[offIndex++];
        while(!table[tmp].empty()) {
            fptr << table[tmp].front().url << std::endl;
            delete [] table[tmp].front().url;
            delete [] table[tmp].front().md5;
            table[tmp].erase(table[tmp].begin());
        }
    }
}