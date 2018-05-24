/* 2018.05.06 ok */

# include "graph.h"

void Graph:: add_edge(int u, int v) {
    edge.push_back((Edge){u, v, headlist[u]});
    headlist[u] = edge.size() - 1;
    dout[u] ++, din[v] ++;
    return;
}

/*
void Graph:: add_edge(const std::string &u, const std::string &v) {
    if(!label.count(u)) label[u] = id ++;
    if(!label.count(v)) label[v] = id ++;
    add_edge(label[u], label[v]);
    // edge.push_back((Edge){u, v, headlist[u]});
    // headlist[u] = edge.size() - 1;
    return;
}
 */