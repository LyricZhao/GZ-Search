//
// Created by Lyric Zhao on 2018/5/20.
//

# include <vector>
# include <cstdio>
# include <string>
# include <cstring>
# include <fstream>
# include <db_cxx.h>
# include <iostream>

# include "utility.h"
# include "../wSplit/api.h"

std:: string gbk2utf8(const char* in);
std:: string utf82gbk(std:: string in);

void print_sp_w0(char *str, int len) {
    char *s = new char[len + 1];
    memcpy(s, str, len);
    s[len] = 0;
    std:: cout << s << std:: endl;
    delete []s;
    return;
}

void checkContext() {
    std:: string db_name = "context.db";
    Db *db = new Db(NULL, 0);
    try {
        db -> open(NULL, db_name.c_str(), NULL, DB_HASH, DB_CREATE, 0);
    } catch (DbException &exc) {
        std:: cout << exc.what() << std:: endl;
    }

    Dbc *cursor;
    db -> cursor(NULL, &cursor, 0);

    Dbt key, data;
    while((cursor -> get(&key, &data, DB_NEXT)) != DB_NOTFOUND) {
        int *key_str = (int *) key.get_data();
        char *con_str = (char *) data.get_data();
        std:: cout << "* KEY: " << *key_str << std:: endl;
        CONTEXT context(con_str);
        context.debugPrint();
    }

    return;
}

void checkURL() {
    std:: string db_name = "url2id.db";
    Db *db = new Db(NULL, 0);
    try {
        db -> open(NULL, db_name.c_str(), NULL, DB_HASH, DB_CREATE, 0);
    } catch(DbException &exc)  {
        std:: cout << exc.what() << std:: endl;
    }

    Dbc *cursor;
    db -> cursor(NULL, &cursor, 0);

    Dbt key, data;
    while((cursor -> get(&key, &data, DB_NEXT)) != DB_NOTFOUND) {
        char *key_str = (char *) key.get_data();
        int *int_str = (int *) data.get_data();

        std:: cout << "* KEY: ";
        print_sp_w0(key_str, key.get_size());

        std:: cout << "* VAL: " << *int_str << std:: endl;
        std:: cout << std:: endl;
    }
    return;
}

void checkDict() {
    std:: string db_name = "worddict.db";
    Db *db = new Db(NULL, 0);
    try {
        db -> open(NULL, db_name.c_str(), NULL, DB_HASH, DB_CREATE, 0);
    } catch(DbException &exc)  {
        std:: cout << exc.what() << std:: endl;
    }

    Dbc *cursor;
    db -> cursor(NULL, &cursor, 0);

    Dbt key, data;
    while((cursor -> get(&key, &data, DB_NEXT)) != DB_NOTFOUND) {
        char *key_str = (char *) key.get_data();
        int *int_str = (int *) data.get_data();

        std:: cout << "* KEY: ";
        print_sp_w0(key_str, key.get_size());

        std:: cout << "* VAL: " << *int_str << std:: endl;
        std:: cout << std:: endl;
    }
    return;
}

int main() {
    checkContext();
    return 0;
}
