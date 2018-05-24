/* 2018.05.06 ok */

# include <string>
# include <iostream>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

# include "utility.h"

FLAGWLOCK FLAGS;

pthread_mutex_t printMutex;
pthread_mutex_t databaseWMutex;
pthread_mutex_t databaseIMutex;

/*
class CONTEXT {
  int type, id;
  std:: string title, url, context;
  std:: vector<std:: string> related;
  CONTEXT(const unsigned char *recvSTR, int id);
};
*/

void stringSpilt(const std:: string &str, const std:: string iChar, std:: vector<std:: string> &Vec) {
  int currentPos = 0, nextPos, size = str.size();
  while((nextPos = str.find(iChar, currentPos)) != std:: string:: npos) {
    if(currentPos != nextPos - 1) {
      Vec.push_back(str.substr(currentPos, nextPos - currentPos));
    }
    currentPos = nextPos + 1;
  }
  if(currentPos != size) {
    Vec.push_back(str.substr(currentPos, size - currentPos));
  }
  return;
}

void CONTEXT:: packString(char* &tempBuffer, const std:: string &str) {
 	int strSize = str.size() + 1;
 	memcpy(tempBuffer, str.c_str(), strSize);
    tempBuffer += strSize;
 	return;
}

int CONTEXT:: getTotalSize() {
  int ret = 0;
  ret += sizeof(int); // type
  ret += sizeof(int); // id
  ret += sizeof(double); // pr
  ret += title.size() + 1; // title
  ret += url.size() + 1; // url
  ret += context.size() + 1; // context
  ret += sizeof(int); // the size of the vector
  for(auto entry: related) {
    ret += entry.size() + 1; // related url
  }
  return ret;
}

char* CONTEXT:: getBuffer(int id) {
  if(buffer != NULL) {
    return buffer;
  }
  // PRINT(std:: cout << "Computing buffer" << std:: endl);
  bufLength = getTotalSize();
  buffer = (char *) malloc(sizeof(char) * bufLength);
  memset(buffer, 0, sizeof(char) * bufLength);

  char *tempBuffer = buffer;
  memcpy(tempBuffer, &type, sizeof(int)); tempBuffer += sizeof(int);
  pid = id; memcpy(tempBuffer, &id, sizeof(int)); tempBuffer += sizeof(int);
  memcpy(tempBuffer, &pr, sizeof(double)); tempBuffer += sizeof(double);
  packString(tempBuffer, title);
  packString(tempBuffer, url);
  packString(tempBuffer, context);
  int vSize = related.size();
  memcpy(tempBuffer, &vSize, sizeof(int)); tempBuffer += sizeof(int);
  for(auto entry: related) {
    packString(tempBuffer, entry);
  }

  return buffer;
}

void CONTEXT:: debugPrint() {
  PRINT(std:: cout << "$ Print Context ID #" << (this -> pid) << ":" << std:: endl);
  PRINT(std:: cout << " # type: " << (this -> type) << std:: endl);
  PRINT(std:: cout << " # title: " << (this -> title) << std:: endl);
  PRINT(std:: cout << " # pagerank: " << (this -> pr) << std:: endl);
  PRINT(std:: cout << " # url: " << (this -> url) << std:: endl);
  PRINT(std:: cout << " # context: " << (this -> context) << std:: endl);
  PRINT(std:: cout << " # related: " << std:: endl);
  for(auto it: (this -> related)) {
    PRINT(std:: cout << "  &:" << it << std:: endl);
  }
  PRINT(std:: cout << std:: endl);
  return;
}

// id will be overwritten later.
CONTEXT:: CONTEXT(const unsigned char *str, int id) {
  std:: string recvSTR = (char *)str;
  int typeS = recvSTR.find("@GZ_TYPE@") + 9;
  int typeE = recvSTR.find("@GZ_TITLE@");
  int titleS = recvSTR.find("@GZ_TITLE@") + 10;
  int titleE = recvSTR.find("@GZ_URL@");
  int urlS = recvSTR.find("@GZ_URL@") + 8;
  int urlE = recvSTR.find("@GZ_CONTEXT@");
  int contextS = recvSTR.find("@GZ_CONTEXT@") + 12;
  int contextE = recvSTR.find("@GZ_RELATED@");
  int relatedS = recvSTR.find("@GZ_RELATED@") + 12;
  int relatedE = recvSTR.find("@GZ_END@");
  // PRINT(std:: cout << relatedS << " " << relatedE << std:: endl);

  this -> pid = id; // this id is related in the url2id database
  this -> type = atoi(recvSTR.substr(typeS, typeE - typeS).c_str());
  this -> pr = 0; // this will be overwritten later (cpr stage).
  this -> title = recvSTR.substr(titleS, titleE - titleS);
  this -> url = recvSTR.substr(urlS, urlE - urlS);
  this -> context = recvSTR.substr(contextS, contextE - contextS);

  std:: string allRelated = recvSTR.substr(relatedS, relatedE - relatedS);
  (this -> related).clear();
  stringSpilt(allRelated, ",", this -> related);

  buffer = NULL;
  bufLength = 0;
  // debugPrint();

  return;
}

CONTEXT::CONTEXT(void *data) { // convert back
    char *temp = (char *)data;
    // int length = strlen(temp);
    // PRINT(std:: cout << "the length is that " << length << std:: endl);
    type = CV(int, temp); temp += sizeof(int);
    pid = CV(int, temp); temp += sizeof(int);
    pr = CV(double, temp); temp += sizeof(double);
    title = temp; temp += title.length() + 1;
    url = temp; temp += url.length() + 1;
    context = temp; temp += context.length() + 1;
    related.clear();
    int sz = CV(int, temp); temp += sizeof(int);
    for(int i = 0; i < sz; ++ i) {
        std:: string tempUrl;
        tempUrl = temp; temp += tempUrl.length() + 1;
        related.push_back(tempUrl);
    }
    buffer = NULL;
    bufLength = 0;
    return;
}

CONTEXT::~CONTEXT() {
    if(buffer != NULL) {
        free(NULL);
    }
    related.clear();
}

void FLAGWLOCK:: init() {
  EXIT_FLAG = false;
  PR_IS_COMPUTING_FLAG = false;
  DATABASE_IS_WRITING_FLAG = false;
  DONT_RECV_DATA = false;
  std:: cout << "flagwlock init" << std:: endl;
  pthread_mutex_init(&Mutex, NULL);
  return;
}

void FLAGWLOCK:: destroy() {
  pthread_mutex_destroy(&Mutex);
  return;
}

void FLAGWLOCK:: lock() {
  pthread_mutex_lock(&Mutex);
  return;
}

void FLAGWLOCK:: unlock() {
  pthread_mutex_unlock(&Mutex);
  return;
}

void lockInit() {
  FLAGS.init();
  pthread_mutex_init(&printMutex, NULL);
  pthread_mutex_init(&databaseWMutex, NULL);
  pthread_mutex_init(&databaseIMutex, NULL);
  return;
}

void lockEnd() {
  FLAGS.destroy();
  pthread_mutex_destroy(&printMutex);
  pthread_mutex_destroy(&databaseWMutex);
  pthread_mutex_destroy(&databaseIMutex);
  return;
}

bool getDWILock() {
  pthread_mutex_lock(&databaseIMutex);
  return true;
}

bool releaseDBILock() {
  pthread_mutex_unlock(&databaseIMutex);
  return true;
}

bool getDBWLock() {
  pthread_mutex_lock(&databaseWMutex);
  return true;
}

bool releaseDBWLock() {
  pthread_mutex_unlock(&databaseWMutex);
  return true;
}

bool getPrintLock() {
  pthread_mutex_lock(&printMutex);
  return true;
}

bool releasePrintLock() {
  pthread_mutex_unlock(&printMutex);
  return true;
}
