/*
The basic graph is witten in template, so the definition and declaration
should be put in the same file
*/
#ifndef BASICGRAPH_
#define BASICGRAPH_
#include "vertex.h"
#include "edge.h"
#include <vector>
#include <list>
using std::vector;
using std::list;

template <typename VT, typename ET>
class BasicGraph {
public:
  BasicGraph(): V_(0), E_(0) {}
  ~BasicGraph();
  Vertex<VT>* vertex(int v) { return vertex_[v]; }
  Edge<ET>* edge(int e) { return edge_[e]; }
  bool VertexExist(const VT& value) const;
  int V() const { return V_; }
  int E() const { return E_; }
  bool isempty() { return V_; } //wether the graph is empty

  int addEdge(const ET& value, int pv, int nv); //return the number of edge
  int addVertex(const VT& value); // return the number of vertex
  list<int>::iterator begin(int v) { return adjList[v].begin(); }
  list<int>::iterator end(int v) { return adjList[v].end(); }

protected:
  virtual double newPrio(const ET& ) = 0;    //how to define the priority depends on the sepcific kind of graph
  int V_;
  int E_;
  vector< Vertex<VT>* > vertex_;            // all vertexs
  vector< Edge<ET>* > edge_;                //all edges
  vector< list<int> > adjList;             //adjacent list
};

/*-----------------------------functions--------------------*/

template <typename VT, typename ET>
bool BasicGraph<VT, ET>::VertexExist(const VT& value) const{
  for (int i = 0; i < V_; i++) {
    if (value == vertex_[i] -> value()) return true;
  }
  return false;
}

template <typename VT, typename ET>
BasicGraph<VT, ET>::~BasicGraph() {
  for (int i = 0; i < V_; i++) {
    delete vertex_[i];
  }
  for (int i = 0; i < E_; i++) {
    delete edge_[i];
  }
}

template <typename VT, typename ET>
int BasicGraph<VT, ET>::addEdge(const ET& value, int pv, int nv) {
  Edge<ET>* eg = new Edge<ET>(value, pv, nv);
  edge_.push_back(eg);
  adjList[pv].push_back(E_);
  E_++;
  vertex_[pv] -> addpEdge();
  vertex_[nv] -> addnEdge();
}

template <typename VT, typename ET>
int BasicGraph<VT, ET>::addVertex(const VT& value) {
  Vertex<VT>* ver = new Vertex<VT>(value);
  vertex_.push_back(ver);
  list<int> vList;
  adjList.push_back(vList);
  V_++;
}

#endif //BAISCGRAPH_
