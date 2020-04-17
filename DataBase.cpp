//
// Created by Ian Liu on 2020/4/15.
//

#include "DataBase.h"

DataBase::DataBase(): sess("localhost", 33060, "root", "cht123456") {
    sess.sql("USE craweler").execute();
}


bool DataBase::query(std::string md5) {
    std::string query = "SELECT S.md5 from seendb S Where S.md5=\'" + md5 + "\'";
    auto result = sess.sql(query).execute();
    mysqlx::Row row = result.fetchOne();
    if(row[0].isNull()) return false;
    return true;
}

void DataBase::insert(std::string md5) {
    std::string query = "INSERT INTO seendb values(\'" + md5 + "\')";
    sess.sql(query).execute();
}