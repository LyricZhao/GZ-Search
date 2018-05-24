#ifndef VERTEX_
#define VERTEX_

template <typename T>
class Vertex {
public:
  static const constexpr double minPrio = 0;
  static const constexpr double maxPrio = 0x7fffffff;
  Vertex(const T& value): value_(value), priority_(maxPrio), visit_(false), pd_(0), nd_(0) {}
  Vertex(): priority_(maxPrio), visit_(false), pd_(0), nd_(0) {}
  T value() const { return value_; }
  double& priority() { return priority_; }
  void hasvisit() { visit_ = true; }
  void notvisit() { visit_ = false; }
  bool visit() const { return visit_; }
  int pd() const { return pd_; }
  int nd() const { return nd_; }
  int addpEdge() { return ++pd_; }
  int addnEdge() { return ++nd_; }

private:
  T value_;
  double priority_;
  bool visit_;
  int pd_;
  int nd_;
};

/*---------------functions--------------------*/



#endif //VERTEX_
