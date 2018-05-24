//
// Created by Lyric Zhao on 2018/5/2.
//

# include <algorithm>

# include "context.h"

CONTEXT::CONTEXT(void *data) {
    char *temp = (char *)data;
    type = CV(int, temp); temp += sizeof(int);
    pid = CV(int, temp); temp += sizeof(int);
    pr = CV(double, temp); temp += sizeof(double);
    title = temp; temp += title.length() + 1;
    url = temp; temp += url.length() + 1;
    context = temp; temp += context.length() + 1;
    related.clear();
    int sz = CV(int, temp);
    for(int i = 0; i < sz; ++ i) {
        std:: string tempUrl;
        tempUrl = temp; temp += tempUrl.length() + 1;
        related.push_back(tempUrl);
    }
    return;
}

CONTEXT::~CONTEXT() {
    related.clear();
}

void CONTEXT::fitIN(userCONTEXT &uc) {
    uc.url = url;
    uc.title = title;
    uc.context = context.substr(0, std:: min(MAXN_RTN_LENGTH, (int)(context.length())));
    return;
}