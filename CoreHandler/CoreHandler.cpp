/* CoreHandler Main */
/* 2018.05.06 ok */

# include <string>
# include <stdio.h>
# include <assert.h>
# include <iostream>
# include <pthread.h>

# include "utility.h"
# include "inputHandler.h"
# include "prComputer.h"
# include "networkRecv.h"
# include "dataBaseWriter.h"

int main() {
	/* Program starts */
	std:: cout << "GZ Search CoreHandler started." << std:: endl;
	lockInit();

	/* Process the input of the manager */
	pthread_t inputHandler;
	assert(pthread_create(&inputHandler, NULL, &inputHandlerENT, NULL) == 0);

	/* Calculate the PageRank */
	pthread_t prComputer;
	assert(pthread_create(&prComputer, NULL, &prComputerENT, NULL) == 0);

	/* Receive data from Spider through ZeroMQ */
	pthread_t networkRecv;
	assert(pthread_create(&networkRecv, NULL, &networkRecvENT, NULL) == 0);

	/* Write data into database */
	pthread_t dataBaseWriter;
	assert(pthread_create(&dataBaseWriter, NULL, &dataBaseWriterENT, NULL) == 0);

	/* Program Ends */
	pthread_join(inputHandler, NULL);
	pthread_join(prComputer, NULL);
	pthread_join(networkRecv, NULL);
	pthread_join(dataBaseWriter, NULL);
	lockEnd();

	return 0;
}
