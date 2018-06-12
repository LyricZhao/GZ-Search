//
// Created by Lyric Zhao on 2018/5/2.
//

/* 2018.05.06 ok */

#ifndef REQUESTHANDLER_DATABASE_H
#define REQUESTHANDLER_DATABASE_H

# include <string>
# include <db_cxx.h>

class mBerkeleyDB {
public:
    Db *db;
    mBerkeleyDB() {}
    mBerkeleyDB(const std:: string &filename);
    ~mBerkeleyDB();
    int size();
    void close();
    void sync();
    void getval(void *keyd, int length, Dbt &data);
    bool exist(void *keyd, int length);
    void put(void *keyd, int key_length, void *datad, int data_length);
    void errInfo(const std:: string err);   ``
};

class dbDUP: public mBerkeleyDB {
public:
    dbDUP(const std:: string &filename);
    void put(void *keyd, int key_length, void *datad, int data_length);
};

#endif //REQUESTHANDLER_DATABASE_H
