#ifndef WORDGRAPH_
#define WORDGRAPH_

#include "graph/basicgraph.h"
#include "Word.h"
#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>

using std::vector;
using std::string;
using std::cout;
using std::list;

class WordGraph: public BasicGraph<int, Word> {
public:
  WordGraph();
  WordGraph(const string& text);
  WordGraph(const string& text, Dict& dictionary);      // the tex from which the graph is going to bulid
  ~WordGraph();
  bool build(Dict& dict);
  void Dijkstra ();
  const int* fatherEdge() const { return fatherEdge_; }
  double minRoute(int v) const; // start from vertex 0
private:
  virtual double newPrio(const Word& value) {}
  string text_;
  int* fatherEdge_;
};


#endif //WORDGRAPH_
