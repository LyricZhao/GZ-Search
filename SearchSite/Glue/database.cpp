//
// Created by Lyric Zhao on 2018/5/2.
//

/* 2018.05.06 ok */

# include <db_cxx.h>

# include "database.h"

mBerkeleyDB:: mBerkeleyDB(const std:: string &filename) {
    db = new Db(NULL, 0);
    try {
        db -> open(NULL, filename.c_str(), NULL, DB_HASH, DB_CREATE, 0);
    } catch(DbException &exc) {
        errInfo(exc.what());
    }
    return;
}

mBerkeleyDB:: ~mBerkeleyDB() {
    if(db == NULL) return;
    try {
        db -> close(0);
    } catch(DbException &exc) {
        errInfo(exc.what());
    }
    delete db;
    db = NULL;
    return;
}

void mBerkeleyDB:: sync() {
    db -> sync(0);
    return;
}

void mBerkeleyDB:: close() {
    if(db == NULL) return;
    try {
        db -> close(0);
    } catch(DbException &exc) {
        errInfo(exc.what());
    }
    delete db;
    db = NULL;
    return;
}

void mBerkeleyDB:: getval(void *keyd, int length, Dbt &data) {
    /*
    Dbt key(keyd, length);
    Dbc *cursor;

    db -> cursor(NULL, &cursor, 0);
    int ret = cursor -> get(&key, &data, DB_SET);
    PRINT(std:: cout << "finding ret = " << (ret != DB_NOTFOUND) << std:: endl);

    cursor -> close();*/

    Dbt key(keyd, length);

    int ret = db -> get(NULL, &key, &data, 0);
    // PRINT_VAR(&data);
    // PRINT(std:: cout << "finding ret = " << (ret != DB_NOTFOUND) << std:: endl);

    return;
}

void mBerkeleyDB:: errInfo(const std:: string err) {
    std:: cerr << err << std:: endl;
    return;
}

bool mBerkeleyDB:: exist(void *keyd, int length) {
    Dbc *cursor;
    db -> cursor(NULL, &cursor, 0);

    Dbt key(keyd, length), getData;
    int ret = cursor -> get(&key, &getData, DB_SET);

    cursor -> close();
    return ret != DB_NOTFOUND;
}

void mBerkeleyDB:: put(void *keyd, int key_length, void *datad, int data_length) {
    Dbt key(keyd, key_length);
    Dbt data(datad, data_length);

    int ret = db -> put(NULL, &key, &data, 0);
    // PRINT(std:: cout << "putting ret = " << ret << std:: endl);

    return;
}


int mBerkeleyDB:: size() {
    DB_HASH_STAT *sp;
    int ret = db -> stat(NULL, &sp, 0);
    return sp -> hash_nkeys;
}

dbDUP:: dbDUP(const std:: string &filename) {
    db = new Db(NULL, 0);
    db -> set_flags(DB_DUP); // important
    try {
        db -> open(NULL, filename.c_str(), NULL, DB_HASH, DB_CREATE, 0);
    } catch(DbException &exc) {
        errInfo(exc.what());
    }
    return;
}