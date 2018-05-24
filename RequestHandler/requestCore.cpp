//
// Created by Lyric Zhao on 2018/5/2.
//

# include <iostream>

# include "requestCore.h"

// Db *globalDB, *wordDict;

void requestCore:: init() {
    globalDB = new mBerkeleyDB("../CoreHandler/context.db");
    wordDict = new dbDUP("../CoreHandler/worddict.db");
    token = new CJKTokenizer("../wSplit/Word_library/word2.txt", "../wSplit/Word_library/invalidWord.txt");
    return;
}

void requestCore:: getIN(const std:: string &str, std:: set<int> &v) {
    Dbc *cursor;
    Db *db = wordDict->db;
    db->cursor(NULL, &cursor, 0);

    Dbt key((char *)str.c_str(), str.length()), data;
    int ret = cursor -> get(&key, &data, DB_SET);

    while(ret != DB_NOTFOUND) {
        int rs = *((int *)(data.get_data()));
        v.insert(rs);
        ret = cursor -> get(&key, &data, DB_NEXT_DUP);
    }
    return;
}


void requestCore:: search(const std:: string &keyword, std:: vector<userCONTEXT> &v) {
    std:: vector<std:: pair<std:: string, int> > vec;
    token -> split2word(keyword, vec);

    std:: set<int> allArticles;
    for(auto it: vec) {
        std:: string word = it.first;
        int count = it.second;
        getIN(word, allArticles);
    }

    std:: cout << "Searching " << keyword << " ... ";

    for(auto id: allArticles) {
        int iid = id;
        Dbt getData;
        globalDB -> getval(&iid, sizeof(int), getData);
        CONTEXT context(getData.get_data());
        userCONTEXT res;
        context.fitIN(res);
        v.push_back(res);
    }

    std:: cout << "ok !" << std:: endl;
    std:: cout << "* Result:" << std:: endl;

    for(auto element: v) {
        std:: cout << "^ url     : " << element.url << std:: endl;
        std:: cout << "^ title   : " << element.title << std:: endl;
        std:: cout << "^ context : " << element.context << std:: endl;
        std:: cout << std:: endl;
    }
    return;
}

void requestCore:: vend() {
    delete globalDB;
    delete wordDict;
    delete token;
    return;
}
