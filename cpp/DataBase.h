//
// Created by Ian Liu on 2020/4/15.
//

#ifndef CRAWLER_MULTI_THREAD_DATABASE_H
#define CRAWLER_MULTI_THREAD_DATABASE_H

#include <iostream>
#include <mysqlx/xdevapi.h>


class DataBase {
    mysqlx::Session sess;
public:
    DataBase();
    bool query(std::string md5);
    void insert(std::string md5);
};


#endif //CRAWLER_MULTI_THREAD_DATABASE_H
