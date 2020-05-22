/* 

Name: crawler

Purpose:
    crawel news website to favor the requriment of data preprocessing
Usage:
    . place desired crawled website under /init/seed.log
    . sample code
        int main() {
            crawler master;
            master.start(); // happy craweling :)
        }

Result:
    . crawled data be placed under result/record.rec
    . format of record.rec
        . NEW_REC:
        . url:
        . tiltle:
        . size:
        . publish date:
        . view:
        . category:
        . body:

History:
    Written by Ian Liu 5/2020

*/

#ifndef CRAWLER_H
#define CRAWLER_H

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

class crawler {
    std::vector<char*> commitQueue;
    void write_out_commit_queue(std::string filePath);
public:
    ~crawler();
    int start();
};


#endif //CRAWLER_MULTI_THREAD_MASTER_H