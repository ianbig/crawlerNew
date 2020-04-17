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
#include <assert.h>
#include "DataBase.h"
#include "gumbo.h"
#include "md5/src/md5.h"


#define FETCH_SUCCESS 0
#define ERROR_MAINTEX_NULL -1
#define ERROR_BLACKLIST_BLOCK -2

struct MemoryStruct {
    char *buffer;
    size_t size;
    bool flag;
    char *url;
    char *title;
    MemoryStruct() {
        buffer = NULL;
        size = false;
        flag = 0;
        url = NULL;
        title = NULL;
    }
    ~MemoryStruct() {
        if(buffer != NULL)  {
            delete [] buffer;
            buffer = NULL;
        }
        if(url != NULL) {
            delete [] url;
            url = NULL;
        }
        if(title != NULL) {
            delete [] title;
            title = NULL;
        }
    }
    MemoryStruct(const MemoryStruct &tmp) {
        buffer = new char[strlen(tmp.buffer) + 1];
        url = new char[strlen(tmp.url) + 1];
        title = new char[strlen(tmp.title) + 1];

        memmove(buffer, tmp.buffer, strlen(tmp.buffer));
        memmove(url, tmp.url, strlen(tmp.url));
        memmove(title, tmp.title, strlen(tmp.title));
        buffer[strlen(tmp.buffer)] = 0;
        url[strlen(tmp.url)] = 0;
        title[strlen(tmp.title)] = 0;
        size = tmp.size;
        flag = tmp.flag;
    }
    struct MemoryStruct& operator=(struct MemoryStruct &rhs) {
        if(this == &rhs) return *this;

        if(buffer != NULL) {
            delete [] buffer;
        }

        if(url != NULL) {
            delete [] url;
        }

        if(title != NULL) {
            delete [] title;
        }
        buffer = new char[strlen(rhs.buffer) + 1];
        url = new char[strlen(rhs.url) + 1];
        title = new char[strlen(rhs.title) + 1];
        memmove(buffer, rhs.buffer, strlen(rhs.buffer));
        memmove(url, rhs.url, strlen(rhs.url));
        memmove(title, rhs.title, strlen(rhs.title));
        buffer[strlen(rhs.buffer)] = 0;
        url[strlen(rhs.url)] = 0;
        title[strlen(rhs.title)] = 0;
        size = rhs.size;
        flag = rhs.flag;

        return *this;
    }
};

class Parser {
    std::vector<struct MemoryStruct> RecordQueue;
    std::vector<std::string> whiteList;
    std::vector<std::string> blackList;
    size_t Size_OF_RecordQueue;
    std::vector<std::string> Parser_Master_Cache;
    DataBase tb;
    std::string cleantext(GumboNode* node);
    void getMainTex(GumboNode *root, char *url);
    void textExtract(GumboNode *root, char *url);
    GumboNode* diveNode(GumboNode *child_root, const char *desiredValue);

public:
    Parser();
    int fetch(const char *url);
    size_t extractLink(char *buffer);
    int mainTexExtration(char *buffer, char *url);
    void write_out_record(std::string filePath);
    void write_out_url(std::string filePath);
    size_t get_record_queue_size();
};


size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
size_t findCaseInsensitive(std::string data, std::string toSearch);
#endif //CRAWLER_MULTI_THREAD_PARSER_H
