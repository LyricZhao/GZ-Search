#ifndef EDGE_
#define EDGE_

template <typename T>
class Edge {
public:
  Edge(const T& value, int pv, int nv): value_(value), pv_(pv), nv_(nv) {}
  Edge(): pv_(0), nv_(0) {}
  T value() const { return value_; }
  int pv() const { return pv_; }
  int nv() const { return nv_; }

  friend bool operator==(const Edge& e1, const Edge& e2) { return e1.value_ == e2.value_; }
  friend bool operator< (const Edge& e1, const Edge& e2) { return e1.value_ < e2.value_; }
  friend bool operator> (const Edge& e1, const Edge& e2) { return e1.value_ > e2.value_; }
private:
  T value_;
  int pv_;
  int nv_;
};


#endif //EDGE_
