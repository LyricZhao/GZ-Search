/* 2018.05.06 ok */

// # include <fcntl.h>
# include <string>
# include <zmq.hpp>
# include <stdio.h>
# include <iostream>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <assert.h>
# include <pthread.h>

# include "utility.h"
# include "networkRecv.h"
# include "dataBaseWriter.h"

# define MAX_RECV_ENTRY 10000
// # define MAX_MSG_SIZE 1048576 // 1 MB

extern FLAGWLOCK FLAGS;

void *networkRecvENT(void *data) {

  PRINT(std:: cout << "networkRecvENT() successfully started." << std:: endl);
  // char *buf = (char *) malloc(sizeof(char) * MAX_MSG_SIZE);

  zmq:: context_t context(1);
  zmq:: socket_t socket(context, ZMQ_REP);
  socket.bind("tcp://*:4723"); // special port: 4723

  int loop_count = 0;
  int recv_count = 0;

  while(true) {
    // PRINT(std:: cout << "Checking network data ... " << ++ loop_count << std:: endl);
    FLAGS.lock();
    // PRINT(std:: cout << "Lock pass" << std:: endl);
    if(FLAGS.EXIT_FLAG == true) {
      FLAGS.unlock();
      goto EXIT_LABEL;
    }
    FLAGS.unlock();
    // PRINT(std:: cout << "Unlock pass" << std:: endl);

    zmq:: message_t recvMsg;
    int rcErr = socket.recv(&recvMsg, ZMQ_DONTWAIT);

    // PRINT(std:: cout << "Recv pass" << std:: endl);

    if(rcErr > 0) {
      zmq:: message_t reply(17);
      memcpy(reply.data(), "CoreHandler gets.", 17);
      socket.send(reply);
      PRINT(std:: cout << "Network thread received a data string from Spider. (" << ++ recv_count << ")" << std:: endl);
      // std:: string recvSTR = recvMsg.str();
      // PRINT(std:: cout << recvSTR << std:: endl);
      unsigned char *recvSTR = recvMsg.data<unsigned char>();
      extern int currentTWEntry;
      FLAGS.lock();
      if(currentTWEntry < MAX_RECV_ENTRY && FLAGS.DONT_RECV_DATA == false) {
        FLAGS.unlock();
        addContext(recvSTR);
      } else {
        FLAGS.unlock();
      }
      // PRINT(std:: cout << recvSTR << std:: endl);
    }
    // PRINT(std:: cout << "Going into sleep" << std:: endl);
    sleep(2);
  }
  EXIT_LABEL:
  // free(buf);
  pthread_exit(0);
}
