/* 2018.05.06 ok */

# ifndef __UTILITY_H__
# define __UTILITY_H__

# define PRINT(str) getPrintLock();str;releasePrintLock()
# define VAR_NAME(var) (#var)
# define PRINT_VAR(var) PRINT(std:: cout << VAR_NAME(var) << " = " << (var) << std:: endl)

# include <vector>
# include <string>

# define CV(TYPE, BUF) (*((TYPE *)(BUF)))

struct FLAGWLOCK {
  bool EXIT_FLAG;
  bool PR_IS_COMPUTING_FLAG;
  bool DATABASE_IS_WRITING_FLAG;
  bool DONT_RECV_DATA;
  pthread_mutex_t Mutex;
  void init();
  void lock();
  void unlock();
  void destroy();
};

// # @GZ_TYPE@...@GZ_TITLE@...@GZ_URL@...@GZ_CONTEXT@...@GZ_RELATED@...@

class CONTEXT {
public:
  int type, pid; double pr;
  std:: string title, url, context;
  std:: vector<std:: string> related; // include a size
  char *buffer;
  int bufLength;
  CONTEXT(const unsigned char *str, int id);
  CONTEXT(void *data);
  ~CONTEXT();
  void debugPrint();
  char *getBuffer(int id);
private:
  int getTotalSize();
  void packString(char* &tempBuffer, const std:: string &str);
};

void stringSpilt(const std:: string &str, const std:: string iChar, std:: vector<std:: string> &Vec);

void lockInit();
void lockEnd();
bool getDBWLock();
bool getDWILock();
bool getPrintLock();
bool releaseDBWLock();
bool releaseDBILock();
bool releasePrintLock();

# endif
