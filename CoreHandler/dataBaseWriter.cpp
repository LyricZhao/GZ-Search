/* 2018.05.06 ok */

# include <vector>
# include <stdio.h>
# include <fstream>
# include <unistd.h>
# include <iostream>
# include <db_cxx.h>
# include <pthread.h>

# include "../wSplit/api.h"
# include "database.h"
# include "utility.h"
# include "dataBaseWriter.h"

extern FLAGWLOCK FLAGS;

std:: vector<CONTEXT> writeQueue;
std:: vector<CONTEXT> tobeW;
int currentTWEntry;

mBerkeleyDB *globalDB;
mBerkeleyDB *url2id;
mBerkeleyDB *wordDict;

CJKTokenizer *token;

// DBW Lock is the lock of writeQueue & tobeW
// writeQueue is a buffer
// tobeW is the connection with database
// DBI Lock is the lock of url2id, globalDB

// # @GZ_TYPE@...@GZ_TITLE@...@GZ_URL@...@GZ_CONTEXT@...@GZ_RELATED@...@
void addContext(unsigned char *recvSTR) {
  CONTEXT nContext(recvSTR, 0);
  // PRINT(std:: cout << "Printing the orginal context" << std:: endl);
  // PRINT(std:: cout << recvSTR << std:: endl);
  getDBWLock();
  ++ currentTWEntry;
  writeQueue.push_back(nContext);
  releaseDBWLock();
  return;
}

int getUrlId(const std:: string &url) {
    char *str = (char *)url.c_str();
    int len = url.length();
    Dbt data;

    if(url2id -> exist(str, len)) {
        url2id -> getval(str, len, data);
        int *ret = (int *)data.get_data();
        return *ret;
    } else {
        int ret = url2id -> size();
        ++ ret;
        url2id -> put(str, len, &ret, sizeof(int));
        return ret;
    }
}

void debug_write_file(const std:: string all) {
    std:: ofstream output_file;
    output_file.open("proutf.txt");
    output_file << all;
    output_file.close();
    return;
}

void writeEntry(CONTEXT &context) {
  // PRINT(std:: cout << "Writing entry:" << std:: endl);
  int id = getUrlId(context.url);
  // PRINT(std:: cout << "The ID got from db is " << id << std:: endl);
  // context.debugPrint();

  // write context
  context.getBuffer(id);
  // PRINT(std:: cout << "buflength = " << context.bufLength << std:: endl);
  globalDB -> put(&id, sizeof(int), context.getBuffer(id), context.bufLength);



  // write word
  std:: vector<std:: pair<std:: string, int> > result;
  std:: string all = context.title + context.title + context.title + context.context; // stupid but simple method

  // PRINT(std:: cout << "Debuging here" << std:: endl);
  // PRINT(std:: cout << all << std:: endl);

  // debug_write_file(all);
  // PRINT_VAR(all);
  // PRINT(std:: cout << "processing words ... ");
  token -> split2word(all, result);
  // PRINT(std:: cout << " ok !" << std:: endl);
  // PRINT(std:: cout << "Split ok" << std:: endl);

  for(auto pdd: result) {
      std:: string word = pdd.first;
      int count = pdd.second;
      // PRINT(std:: cout << "(" << word << ", " << count << ")" << std:: endl);
      // id | count
      char s[8];
      int *sv = (int *)s;
      sv[0] = id, sv[1] = count;
      wordDict -> put((void *)word.c_str(), word.length(), s, sizeof(int) * 2);
  }

  // PRINT(std:: cout << std:: endl);

  return;
}

void writeDataBase() {
  getDWILock();
  for(auto it: tobeW) {
    writeEntry(it);
  }
  currentTWEntry -= tobeW.size();
  tobeW.clear();
  releaseDBILock();
  return;
}

void checkContext() {
  getDBWLock();
  for(auto entry: writeQueue) {
    tobeW.push_back(entry);
  }
  writeQueue.clear();
  releaseDBWLock();
  return;
}

void *dataBaseWriterENT(void *data) {
  PRINT(std:: cout << "dataBaseWriterENT() successfully started." << std:: endl);

  // one mutex
  url2id = new mBerkeleyDB("url2id.db");
  globalDB = new mBerkeleyDB("context.db");

  // no mutex
  wordDict = new dbDUP("worddict.db");

  PRINT(std:: cout << "Loading word-Split function ...");
  token = new CJKTokenizer("../wSplit/Word_library/word2.txt", "../wSplit/Word_library/invalidWord.txt");
  PRINT(std:: cout << " ok!" << std:: endl);

  while(true) {
    FLAGS.lock();
    if(FLAGS.EXIT_FLAG == true) {
      FLAGS.unlock();
      globalDB -> close();
      url2id -> close();
      wordDict -> close();
      goto EXIT_LABEL;
    } else if(FLAGS.DATABASE_IS_WRITING_FLAG || tobeW.size() >= AUTO_WRITE_LIMIT) {
      FLAGS.DATABASE_IS_WRITING_FLAG = true;
      FLAGS.unlock();
      PRINT(std:: cout << "Start to write ... ");
      writeDataBase();
      globalDB -> sync();
      url2id -> sync();
      wordDict -> sync();
      PRINT(std:: cout << "ok !" << std:: endl);
      FLAGS.lock();
      FLAGS.DATABASE_IS_WRITING_FLAG = false;
      FLAGS.unlock();
    } else {
        FLAGS.unlock();
    }

    checkContext();
    sleep(2);
  }
  EXIT_LABEL:
  delete url2id;
  delete wordDict;
  delete globalDB;
  delete token;

  pthread_exit(0);
}

