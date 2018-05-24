#ifndef TST_
#define TST_
#include "TSTNode.h"
#include <iostream>
#define nullptr 0
using std::cout;

template <typename T>
class TST {
public:
  TST(): size_(0), root_(nullptr), hot_(nullptr) {}
  ~TST() { if (0 < size_) delete root_; }

  /*The status of TSTtree*/
  int size() const { return size_; }
  bool empty() const { return !root_; }

  /*The status of TSTNode*/
  bool isroot(TSTNode<T>* node) const { return !(node -> parent()); }
  bool islchild(TSTNode<T>* node) const { return (!isroot(node)) && (node == (node -> parent()) -> left()); }
  bool isrchild(TSTNode<T>* node) const { return (!isroot(node)) && (node == (node -> parent()) -> right()); }
  int balFac(TSTNode<T>* node) const { return stature(node -> left()) - stature(node -> right()); }
  int stature(TSTNode<T>* node) const { return (node == nullptr) ? -1 : node -> height(); }
  bool balanced(TSTNode<T>* node) const { return ( balFac(node) < 2) && (balFac(node) > -2); }

  /*The pointers related to the current*/
  TSTNode<T>*& parentsptr(TSTNode<T>*& node) { //the reference from the node's parent, root will return root_
    return isroot(node) ? root_ :
         (islchild(node) ? (node -> parent()) -> left() : (node -> parent()) -> right());
  }
  TSTNode<T>*& tallerchild(TSTNode<T>*& node) {
    int lh = stature(node -> left()), rh = stature(node -> right());
    if (lh > rh) return node -> left();
    else if(lh < rh) return node -> right();
    else return islchild(node) ? node -> left() : node -> right();
  }

  /*Some points to the vital components of TSTtree*/
  TSTNode<T>*& root() { return root_; }
  TSTNode<T>* hot() const { return hot_; } // the father of the current node in search

  /*Basic functions*/
  TSTNode<T>* insertroot(const T& value);
  TSTNode<T>* insertleft(TSTNode<T>* x, const T& value); // value inserted as x's left child
  TSTNode<T>* insertright(TSTNode<T>* x, const T& value); //value inserted as x's right child
  TSTNode<T>* insertmid(TSTNode<T>* x, const T& value); // vaue inserted as x's mid child
//  int remove(TSTNode<T>* x); //delete the tree with the root x, return the size of the sub-tree

  /*High level functions*/
  TSTNode<T>*& search(const T& value);
  TSTNode<T>*& search(const T& value, TSTNode<T>*& begin); // probably return the reference of begin so it's reference
  TSTNode<T>*& searchin(TSTNode<T>*& node, const T& value, TSTNode<T>*& hot);
  TSTNode<T>* insert(const T& value);
  TSTNode<T>* insert(const T& value, TSTNode<T>*& begin);
  bool remove(const T& value);

  /*traverse functions*/
  template <typename VST> void travIn_recur (VST& visit, TSTNode<T>* node);
  template <typename VST> void travIn(VST& visit) { travIn_recur(visit, root_); }

protected:
  TSTNode<T>* hot_; //the father of the current node in search;
  TSTNode<T>* rotateAt (TSTNode<T>* v);

private:
  int size_;
  TSTNode<T>* root_;
  int updateHeight(TSTNode<T>* node);
  void updateAbove(TSTNode<T>* node);
  TSTNode<T>* connect34(
    TSTNode<T>* a, TSTNode<T>* b, TSTNode<T>* c,
    TSTNode<T>* T0, TSTNode<T>*T1, TSTNode<T>* T2, TSTNode<T>* T3);
};

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
For function definitions
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
*/

/*------------------Following For private function-------------------------*/
template <typename T>
int TST<T>::updateHeight(TSTNode<T>* node) {
  int lh = stature(node -> left()), rh = stature(node -> right());
  int newh = (lh > rh) ? (lh + 1) : (rh + 1);
  return node -> changeheight(newh);
}

template <typename T>
void TST<T>::updateAbove(TSTNode<T>* node) {
  while (node) {
    updateHeight(node);
    node = node -> parent();
  }
  return ;
}

template <typename T>
TSTNode<T>* TST<T>::connect34(
  TSTNode<T>* a, TSTNode<T>* b, TSTNode<T>* c,
  TSTNode<T>* T0, TSTNode<T>* T1, TSTNode<T>* T2, TSTNode<T>* T3) {
    a -> left() = T0; if(T0) T0 -> parent() = a;
    a -> right() = T1; if(T1) T1 -> parent() = a; updateHeight(a);
    c -> left() = T2; if(T2) T2 -> parent() = c;
    c -> right() = T3; if(T3) T3 -> parent() = c; updateHeight(c);
    b -> left() = a; a -> parent() = b;
    b -> right() = c; c -> parent() = b; updateHeight(b);
    return b;
}
/*-----------------Above For private function----------------------------*/


/*-----------------Following For protected function ------------------------*/
template <typename T>
TSTNode<T>* TST<T>::rotateAt(TSTNode<T>* v) {
  TSTNode<T>* p = v -> parent();
  TSTNode<T>* g = p -> parent();
  if (islchild(p)) {// zig
    if (islchild(v)) {//zig zig
      p -> parent() = g -> parent();
      return connect34(v, p, g, v -> left(), v -> right(), p -> right(), g -> right());
    }
    else {// zig zag
      v -> parent() = g -> parent();
      return connect34(p, v, g, p -> left(), v -> left(), v -> right(), g -> right());
    }
  } else { // zag
    if (islchild(v)) { //zag zig
      v -> parent() = g -> parent();
      return connect34(g, v, p, g -> left(), v -> left(), v -> right(), p -> right());
    }
    else { // zag zag
      p -> parent() = g -> parent();
      return connect34(g, p ,v, g -> left(), p -> left(), v -> left(), v -> right());
    }
  }
}

/*-----------------Above for protected function---------------------------*/

/*-----------------------Following for public function-------------------*/

template <typename T>
template <typename VST>
void TST<T>::travIn_recur (VST& visit, TSTNode<T>* node) {
  if (!node) return;
  travIn_recur(visit, node -> left());
  visit(node -> value());
  travIn_recur(visit, node -> right());
}

template <typename T>
TSTNode<T>* TST<T>::insertroot(const T& value) {
  size_ = 1;
  return root_ = new TSTNode<T>(value);
}

template <typename T>
TSTNode<T>* TST<T>::insertleft(TSTNode<T>* x, const T& value) {
  size_++;
  x -> insertleft(value);
  updateAbove(x);
  return x -> left();
}

template <typename T>
TSTNode<T>* TST<T>::insertright(TSTNode<T>* x, const T& value) {
  size_++;
  x -> insertright(value);
  updateAbove(x);
  return x -> right();
}

template <typename T>
TSTNode<T>* TST<T>::insertmid(TSTNode<T>* x, const T& value) {
  size_++;
  x -> insertmid(value);
  //updateHeight(x);
  return x -> mid();
}

template <typename T>
TSTNode<T>*& TST<T>::searchin(TSTNode<T>*& node, const T& value, TSTNode<T>*& hot) {
  if(!node || (value == node -> value())) return node;
  hot = node;
  return searchin(((value < node -> value()) ? (node -> left()) : (node -> right())), value, hot);
}// return a reference for convenience to modify the node value in insert()

template <typename T>
TSTNode<T>*& TST<T>::search(const T& value) {
  hot_ = nullptr;
  return searchin(root_, value, hot_);
}

template <typename T>
TSTNode<T>*& TST<T>::search(const T& value, TSTNode<T>*& begin) {
  hot_ = nullptr;
  return searchin(begin, value, hot_);
}

template <typename T>
TSTNode<T>* TST<T>::insert(const T& value, TSTNode<T>*& begin) {
  TSTNode<T>*& node = search(value, begin);
  if (node) return node; // the node has already exist.
  TSTNode<T>* t_node = node = new TSTNode<T> (value, hot_);  //t_node is a real point to the new node
  size_++;
  for (TSTNode<T>* g = hot_; g; g = g -> parent()) {
    if(!balanced(g)) {
      if (isroot(g)) {
          begin = rotateAt(tallerchild(tallerchild(g)));
      } else {
        parentsptr(g) = rotateAt(tallerchild(tallerchild(g)));// the caller does the top-to-bottom connection
      }
      break; // if rotate, the ancestors are balanced.
    } else {
      updateHeight(g);
    }
  }
//  updateAbove(node);
  return t_node;
}

template <typename T>
TSTNode<T>* TST<T>::insert(const T& value) {
  return insert(value, root_);
}

/*-------------------Above for public functions ---------------------------*/
#endif //TST_
