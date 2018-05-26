//
// Created by Lyric Zhao on 2018/5/2.
//

#ifndef REQUESTHANDLER_CONTEXT_H
#define REQUESTHANDLER_CONTEXT_H

# include <vector>
# include <string>

# define MAXN_RTN_LENGTH 200
# define CV(TYPE, BUF) (*((TYPE *)(BUF)))

class userCONTEXT {
public:
    int type;
    std:: string url, title, context;
};

class CONTEXT {
public:
    int type, pid; double pr;
    std:: string title, url, context;
    std:: vector<std:: string> related; // include a size
    CONTEXT(void *data);
    ~CONTEXT();
    void fitIN(userCONTEXT &uc);
    // void debugPrint();
};

#endif //REQUESTHANDLER_CONTEXT_H
