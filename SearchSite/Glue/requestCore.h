//
// Created by Lyric Zhao on 2018/5/2.
//

#ifndef REQUESTHANDLER_REQUESTCORE_H
#define REQUESTHANDLER_REQUESTCORE_H

# include "context.h"
# include "../../wSplit/api.h"
# include "database.h"

# include <set>
# include <vector>
# include <string>
# include <algorithm>

class requestCore {
private:
    mBerkeleyDB *globalDB;
    dbDUP *wordDict;
    CJKTokenizer *token;

    void getIN(const std:: string &str, std:: set<int> &v);

public:
    requestCore() {}
    ~requestCore() {vend();}
    void init();
    void search(const std:: string &keyword, std:: vector<userCONTEXT> &v);
    void vend();
};

#endif //REQUESTHANDLER_REQUESTCORE_H
