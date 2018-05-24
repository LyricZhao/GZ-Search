/* 2018.05.06 ok */

#ifndef PROJECT_GRAPH_H
#define PROJECT_GRAPH_H

# include <map>
# include <string>
# include <vector>
# include <cstdlib>
# include <cstring>
# include <cassert>

struct Edge {
    int u, v, nx;
};

class Graph {
private:
    // std:: map<std:: string, int> label;
public:
    std:: vector<Edge> edge;
    int *headlist, *din, *dout, n, id;

    Graph(int _n): n(_n) {
        id = 0;
        edge.clear();
        // label.clear();
        if(n == 0) headlist = din = dout = NULL;
        else {
            headlist = (int *) malloc(n * sizeof(int));
            din = (int *) malloc(n * sizeof(int));
            dout = (int *) malloc(n * sizeof(int));
            memset(headlist, 0, n * sizeof(int));
            memset(din, 0, n * sizeof(int));
            memset(dout, 0, n * sizeof(int));
        }

    }
    void resize(int _n) {
        assert(_n > 0);
        id = 0, n = _n;
        edge.clear();
        // label.clear();
        if(headlist == NULL) {
            headlist = (int *) malloc(n * sizeof(int));
            din = (int *) malloc(n * sizeof(int));
            dout = (int *) malloc(n * sizeof(int));
        }
        else {
            headlist = (int *) realloc(headlist, n * sizeof(int));
            din = (int *) realloc(din, n * sizeof(int));
            dout = (int *) realloc(dout, n * sizeof(int));
        }
        memset(headlist, 0, n * sizeof(int));
        memset(din, 0, n * sizeof(int));
        memset(dout, 0, n * sizeof(int));
        return;
    }
    // void add_edge(std:: string u, std:: string v);
    void add_edge(int u, int v);
    ~Graph() {
        edge.clear();
        // label.clear();
        free(headlist);
        free(din);
        free(dout);
    }
};

#endif //PROJECT_GRAPH_H