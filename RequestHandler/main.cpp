# include <vector>
# include <iostream>
# include <db_cxx.h>

# include "requestCore.h"

int main() {
    requestCore *searchEngine = new requestCore;
    searchEngine -> init();

    std:: vector<userCONTEXT> result;
    searchEngine -> search("记者", result);

    return 0;
}