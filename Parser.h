//
// Created by Ian Liu on 2020/4/7.
//

#ifndef CRAWLER_MULTI_THREAD_PARSER_H
#define CRAWLER_MULTI_THREAD_PARSER_H

#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <curl/easy.h>
#include <vector>
#include <regex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"

#define HTML_QUEUE_SIZE 1073741824
#define HASH_MAX_SIZE 67107000

struct MemoryStruct {
    char *buffer;
    size_t size;
    bool flag;
    char *url;
    MemoryStruct() {
        buffer = NULL;
        size = false;
        flag = 0;
        url = NULL;
    }
    ~MemoryStruct() {
        if(buffer != NULL)  delete [] buffer;
        if(url != NULL) delete [] url;
    }
    MemoryStruct(const MemoryStruct &tmp) {
        buffer = new char[strlen(tmp.buffer) + 1];
        url = new char[strlen(tmp.url) + 1];

        memmove(buffer, tmp.buffer, strlen(tmp.buffer));
        memmove(url, tmp.url, strlen(tmp.url));
        size = tmp.size;
        flag = tmp.flag;
    }
};

class Parser {
    std::vector<struct MemoryStruct> RecordQueue;
    std::vector<std::string> whiteList;
    std::vector<std::string> blackList;
    size_t Size_OF_RecordQueue;
    std::vector<std::string> Parser_Master_Cache;
    HashTable tb;
public:
    Parser();
    int fetch(const char *url);
    size_t extractLink(char *buffer);
    //size_t mainTexExtration(char *buffer);

};


size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
size_t findCaseInsensitive(std::string data, std::string toSearch);
#endif //CRAWLER_MULTI_THREAD_PARSER_H
