//
// Created by Ian Liu on 2020/4/7.
//

#ifndef CRAWLER_MULTI_THREAD_MASTER_H
#define CRAWLER_MULTI_THREAD_MASTER_H

#define MAX_RECORD_SIZE 500000
#define _GNU_SOUARCE
#define MAX_RECORD 1000
#define ERROR_SEED_EMPTY -1
#define FLOW_CONTROL 0.0
#define MAX_URL_CAPACITY 1048576

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>
#include "Parser.h"

#define TV_PER_SEC 1000000

class Master {
    std::vector<char*> commitQueue;
    void write_out_commit_queue(std::string filePath);
public:
    Master();
    ~Master();
    int start();
};


#endif //CRAWLER_MULTI_THREAD_MASTER_H
