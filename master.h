//
// Created by Ian Liu on 2020/4/7.
//

#ifndef CRAWLER_MULTI_THREAD_MASTER_H
#define CRAWLER_MULTI_THREAD_MASTER_H

#define MAX_RECORD_SIZE 2147483648
#define ERROR_SEED_NO_URL -1
#define _GNU_SOUARCE

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "Parser.h"

class Master {
    std::vector<std::string> commitQueue;
    size_t line_length;
    void write_out_commit_queue(std::string filePath);
public:
    Master();
    int start();
};


#endif //CRAWLER_MULTI_THREAD_MASTER_H
