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
#define HASH_MAX_SIZE 1073741824

struct MemoryStruct {
    char *buffer;
    size_t size;
    bool flag;
    std::string url;
};

class Parser {
    std::vector<struct MemoryStruct> htmlQueue;
    std::vector<std::string> whiteList;
    size_t Size_OF_htmlQueue;
    HashTable tb;
public:
    Parser();
    ~Parser();
    int fetch(const char *url);
    size_t extractLink(char *buffer);
    std::vector<std::string> Parser_Master_Cache;
};


size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
#endif //CRAWLER_MULTI_THREAD_PARSER_H
