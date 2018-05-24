/* 2018.05.06 ok */

# ifndef __PRCOMPUTER_H__
# define __PRCOMPUTER_H__

# include "graph.h"

int getID(const std:: string &url);
void computePageRankSetup(Graph *G);
int getGID(int x);
void dataReaderPR(Graph *G);
void dataWriterPR(Graph *G, double *iterRes);
double *iteratePR(Graph *G, double *iterA, double *iterB);
void computePageRank();
void *prComputerENT(void *data);

# endif
