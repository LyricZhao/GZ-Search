#include "WordGraph.h"
#include "Dictionary.h"
#include "Word.h"
#include <queue>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <iostream>
using std::cout;
using std::list;
using std::stack;

WordGraph::WordGraph() {
  fatherEdge_ = new int[0];
}

WordGraph::WordGraph(const string& text): text_(text) {
  fatherEdge_ = new int[0];
}

WordGraph::WordGraph(const string& text, Dict& dict): text_(text) {
  fatherEdge_ = new int[0];
  build(dict);
}

WordGraph::~WordGraph() {
  delete[] fatherEdge_;
}

bool WordGraph::build(Dict& dict) {
  if (!dict.isopen()) {
    cout << "graph bulid faild.\n";
    return false;
  }
  Word wordText(text_);
  // std:: cout << wordText << std:: endl;
  for (int  i = 0; i <= wordText.length(); i++) {
    addVertex(i);
  }
//  std:: cout << "add ver ok" << std:: endl;
  for (int i = 0; i < wordText.length(); i++) {

    //char p = wordText.at(i);
    // std:: cout << (int)(p) << std:: endl;
    if (!dict.getmatch(wordText[i])) {
  //    std:: cout << "in" << std:: endl;
      dict.insertword(wordText[i]);
    //  std:: cout << "out" << std:: endl;
    }
    // std::cout << dict.findResult() << " result\n";
    //std:: cout << "match ok 01" << std:: endl;
    addEdge(dict.findResult(), i, i + 1);
    //std:: cout << "dict ok 02" << std:: endl;
  }
  //std:: cout << "add edge ok" << std:: endl;
  for (int i = 0; i < wordText.length(); i++) {
    for (int j = i + 1; j < wordText.length() && (j - i) <= 8; j++) {
      if (dict.getmatch(wordText.subWord(i, j - i + 1))) {
        addEdge(dict.findResult(), i, j + 1);
      }
    }
  }
  return true;
}

void WordGraph::Dijkstra() {
  double* vertexDis = new double[V_];
  bool* vertexVisit = new bool[V_];
  fatherEdge_ = new int[V_];
  for (int i = 0; i < V_; i++) {
    vertexDis[i] = 1e8;
    vertexVisit[i] = false;
    fatherEdge_[i] = -1;
  }
  vertexDis[0] = 0;
  while (true) {
    int minVertex = -1;
    double minValue = 1e8;
    for (int i = 0; i < V_; i++) {
      if (!vertexVisit[i] && vertexDis[i] < minValue) {
        minVertex = i; minValue = vertexDis[i];
      }
    }
    if (minVertex < 0) break; // all vertex are in
    vertexVisit[minVertex] = true;
    list<int>::iterator it = adjList[minVertex].begin();
    while (it != adjList[minVertex].end()) {
      if (!vertexVisit[edge_[*it] -> nv()] &&
        ((edge_[*it] -> value()).freq() + vertexDis[minVertex] <
                                vertexDis[edge_[*it] -> nv()])  )
      {
        vertexDis[edge_[*it] -> nv()] = (edge_[*it] -> value()).freq() + vertexDis[minVertex];
        fatherEdge_[edge_[*it] -> nv()] = *it;
      }
      it++;
    }
  }
}

double WordGraph::minRoute(int v) const {
 // for (int i = 0; i < V_; i++) cout << fatherEdge_[i] << " ";
  // cout << "\n";
  stack<int> result;
  int ed = fatherEdge_[v], ver = v;
  while (ed != -1) {
    result.push(ed);
    ver = edge_[ed] -> pv();
    ed = fatherEdge_[ver];
  }
  double freq = 0;
  while (!result.empty()) {
    cout << edge_[result.top()] -> value() << "/";
    freq += (edge_[result.top()] -> value()).freq();
    result.pop();
  }
  return freq;
}
