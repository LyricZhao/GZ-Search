/* 2018.05.06 ok */

# include <omp.h>
# include <cmath>
# include <cstdio>
# include <db_cxx.h>
# include <unistd.h>
# include <iostream>
# include <pthread.h>
# include <algorithm>
# include <cassert>
# include "graph.h"
# include "utility.h"
# include "database.h"
# include "prComputer.h"

extern FLAGWLOCK FLAGS;

int iteration_time;
double damping_factor, damping_value, jump_delta = 1e-4;

int tot, *forward_mp, *backward_mp;

extern mBerkeleyDB *globalDB;
extern mBerkeleyDB *url2id;

int getID(const std:: string &url) {
    char *str = (char *)url.c_str();
    int len = url.length();
    Dbt data;

    if(url2id -> exist(str, len)) {
        url2id -> getval((void *)str, len, data);
        int *ret = (int *)data.get_data();
        return *ret;
    } else {
        return -1;
    }
}

void computePageRankSetup(Graph *G) {
    iteration_time = 40;
    damping_factor = 0.85;
    assert(G -> n > 0);
    damping_value = (1.0 - damping_factor) / G -> n;
    return;
}

int getGID(int x) {
    // PRINT(std:: cout << "requesting id = " << x << std:: endl);
    if(forward_mp[x] < 0) {
        forward_mp[x] = tot;
        backward_mp[tot ++] = x;
        return forward_mp[x];
    }
    return forward_mp[x];
}

// the forward map is ranged [1, size]
// the backward map is ranged [0, size - 1]

void dataReaderPR(Graph *G) {
    int size = url2id -> size();
    // PRINT_VAR(url2id -> size());
    forward_mp = (int *) malloc(sizeof(int) * (size + 1));
    backward_mp = (int *) malloc(sizeof(int) * size);

    for(int i = 1; i <= size; ++ i) forward_mp[i] = -1;

    Db *gdb = globalDB -> db;
    Dbc *cursor;
    gdb -> cursor(NULL, &cursor, 0);

    G -> resize(size);

    tot = 0;
    Dbt key, data;

    int mp_tot = 0;

    while((cursor -> get(&key, &data, DB_NEXT)) != DB_NOTFOUND) {
        ++ mp_tot;
        // PRINT_VAR(mp_tot);
        // PRINT(std:: cout << "Here 0" << std:: endl);
        CONTEXT entry(data.get_data());
        // PRINT(std:: cout << "Here 1" << std:: endl);
        int gid = getGID(entry.pid);
        // PRINT(std:: cout << "Here 2" << std:: endl);

        for(auto it: entry.related) {
            // PRINT(std:: cout << "INI" << std:: endl);
            // PRINT_VAR(it);
            // PRINT_VAR(getID(it));

            int cid = getID(it);
            if(cid == -1) continue;

            int vid = getGID(cid);
            G -> add_edge(vid, gid); // reversed edge
            // PRINT(std:: cout << "Adding edge " << vid << " to " << gid << std:: endl);
        }
        // PRINT(std:: cout << "Here 3" << std:: endl);

    }

    // PRINT_VAR(tot);

    return;
}

void dataWriterPR(Graph *G, double *iterRes) {
    for(int i = 0; i < G -> n; ++ i) {
        int rid = backward_mp[i];
        // PRINT(std :: cout << "id = " << i << ", origin = " << rid << std:: endl);
        Dbt data;
        globalDB -> getval(&rid, sizeof(int), data);
        // PRINT_VAR(data.get_size());
        CONTEXT entry(data.get_data());
        entry.pr = iterRes[i];
        entry.getBuffer(rid);
        globalDB -> put(&rid, sizeof(int), entry.getBuffer(rid), entry.bufLength);
        /*
        Dbt afdata;
        globalDB -> getval(&rid, sizeof(int), afdata);
        CONTEXT after(afdata.get_data());
        PRINT(std:: cout << "after change:" << std:: endl);
        after.debugPrint();
        */
    }
    return;
}

double *iteratePR(Graph *G, double *iterA, double *iterB) {
    int n = G -> n;
    for(int i = 0; i < n; ++ i) iterA[i] = 1.0 / n;

    for(int iter = 0; iter < iteration_time; ++ iter) {
        double change = 0;

        # pragma omp parallel for reduction (+:change)
        for(int i = 0; i < n; ++ i) {
            iterB[i] = damping_value;

            for(int eid = G -> headlist[i]; eid; eid = G -> edge[eid].nx) {
                int u = i, v = G -> edge[eid].v;
                assert(G -> din[v] > 0);
                iterB[u] += iterA[v] / (double) G -> din[v];
            }
            change += fabs(iterA[i] - iterB[i]);
        }

        std:: swap(iterA, iterB);
        if(change < jump_delta) break;
    }
    return iterA;
}

void computePageRank() {
  getDWILock();
  Graph *G = new Graph(0);
  dataReaderPR(G);
  releaseDBILock();

  // PRINT(std:: cout << "init ok" << std:: endl);

  computePageRankSetup(G);

  // PRINT(std:: cout << "setup ok" << std:: endl);

  double *iterA, *iterB, *iterRes;
  iterA = (double *) malloc(sizeof(double) * G -> n);
  iterB = (double *) malloc(sizeof(double) * G -> n);
  iterRes = iteratePR(G, iterA, iterB);

  // PRINT(std:: cout << "iter ok" << std:: endl);

//  PRINT(std:: cout << "iteration ok" << std:: endl);

  getDWILock();
  dataWriterPR(G, iterRes);
  // PRINT(std:: cout << "write ok" << std:: endl);
  releaseDBILock();
//  PRINT(std:: cout << "datawriter ok" << std:: endl);

  free(iterA);
  free(iterB);
  free(forward_mp);
  free(backward_mp);
  return;
}

# define OMP_THREADS 20

void *prComputerENT(void *data) {
  PRINT(std:: cout << "prComputerENT() successfully started." << std:: endl);
  omp_set_num_threads(OMP_THREADS);

  while(true) {
    FLAGS.lock();
    if(FLAGS.EXIT_FLAG == true) {
      FLAGS.unlock();
      goto EXIT_LABEL;
    } else if(FLAGS.PR_IS_COMPUTING_FLAG && FLAGS.DATABASE_IS_WRITING_FLAG){
      FLAGS.unlock();
    } else if(FLAGS.PR_IS_COMPUTING_FLAG) {
      FLAGS.unlock();
      PRINT(std:: cout << "Start to compute ... ");
      computePageRank();
      globalDB -> sync();
      url2id -> sync();
      PRINT(std:: cout << "ok !" << std:: endl);
      FLAGS.lock();
      FLAGS.PR_IS_COMPUTING_FLAG = false;
      FLAGS.unlock();
    } else {
        FLAGS.unlock();
    }
    sleep(2);
  }
  EXIT_LABEL:
  pthread_exit(0);
}
