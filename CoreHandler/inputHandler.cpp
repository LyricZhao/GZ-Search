/* 2018.05.06 ok */

# include <string>
# include <stdio.h>
# include <iostream>
# include <pthread.h>

# include "utility.h"
# include "inputHandler.h"

extern FLAGWLOCK FLAGS;

int JudgeType(const std:: string &command) {
  if(command == "exit") { // exit
    return 0;
  } else if(command == "cpr") { // compute pagerank
    return 1;
  } else if(command == "wdb") { // write database
    return 2;
  } else if(command == "status") { // flags
    return 3;
  } else if(command == "stoprecv") { // stop tcp recv
    return 4;
  } else if(command == "conrecv") { // tcp recv
    return 5;
  }
  return -1;
}

void *inputHandlerENT(void *data) {
  PRINT(std:: cout << "inputHandlerENT() successfully started." << std:: endl);
  std:: string command;
  while(std:: cin >> command) {
    switch (JudgeType(command)) {
      /* exit */
      case 0: {
        bool CANT_EXIT_FLAG = false;
        FLAGS.lock();
        if(FLAGS.PR_IS_COMPUTING_FLAG) {
          CANT_EXIT_FLAG = true;
          PRINT(std:: cerr << "PageRank is computing now, you can't exit at this time." << std:: endl);
          break;
        }
        if(FLAGS.DATABASE_IS_WRITING_FLAG) {
          CANT_EXIT_FLAG = true;
          PRINT(std:: cerr << "Database is being written now, you can't exit at this time." << std:: endl);
          break;
        }
        if(!CANT_EXIT_FLAG) {
          FLAGS.EXIT_FLAG = true;
          FLAGS.unlock();
          goto EXIT_LABEL;
        }
        else {
          FLAGS.unlock();
          break;
        }
        break;
      }

      /* cpr */
      case 1: {
        FLAGS.lock();
        if(FLAGS.EXIT_FLAG == true) {
          PRINT(std:: cerr << "'exit' command is being executed.");
        } else if(FLAGS.PR_IS_COMPUTING_FLAG == true) {
          PRINT(std:: cerr << "Pagerank computing is already in process.");
        } else {
          FLAGS.PR_IS_COMPUTING_FLAG = true;
        }
        FLAGS.unlock();
        break;
      }

      /* wdb */
      case 2: {
        FLAGS.lock();
        if(FLAGS.EXIT_FLAG == true) {
          PRINT(std:: cerr << "'exit' command is being executed.");
        } else if(FLAGS.PR_IS_COMPUTING_FLAG == true) {
          PRINT(std:: cerr << "Pagerank computing is in process, you have to wait.");
        } else if(FLAGS.DATABASE_IS_WRITING_FLAG == true){
          PRINT(std:: cerr << "Database writing is already in process.");
        } else {
          FLAGS.DATABASE_IS_WRITING_FLAG = true;
        }
        FLAGS.unlock();
        break;
      }

      /* status */
      case 3: {
        FLAGS.lock();
        PRINT_VAR(FLAGS.EXIT_FLAG);
        PRINT_VAR(FLAGS.PR_IS_COMPUTING_FLAG);
        PRINT_VAR(FLAGS.DATABASE_IS_WRITING_FLAG);
        FLAGS.unlock();
        break;
      }

      /* stoprecv */
      case 4: {
        FLAGS.lock();
        FLAGS.DONT_RECV_DATA = true;
        PRINT(std:: cerr << "TCP connection is disable now." << std:: endl);
        FLAGS.unlock();
        break;
      }

      /* conrecv */
      case 5: {
        FLAGS.lock();
        FLAGS.DONT_RECV_DATA = false;
        PRINT(std:: cerr << "TCP connection is enable now." << std:: endl);
        FLAGS.unlock();
        break;
      }

      default:
        PRINT(std:: cerr << "No such command." << std:: endl);
        break;
    }
  }
  EXIT_LABEL:
  pthread_exit(0);
}
