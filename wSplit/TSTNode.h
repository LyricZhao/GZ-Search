#ifndef TSTNODE_
#define TSTNODE_
#define nullptr 0

template <typename T>
class TSTNode {
public:
  TSTNode():
    left_(nullptr), right_(nullptr), mid_(nullptr), parent_(nullptr), height_(0){}
  TSTNode(const T& value, TSTNode<T>* parent = nullptr, TSTNode<T>* left = nullptr, TSTNode<T>* right = nullptr, TSTNode<T>* mid = nullptr, int h = 0):
    value_(value), parent_(parent), left_(left), right_(right), mid_(mid), height_(h){}
//  int size() const;//the children number of current node
  TSTNode<T>* insertleft(const T& value);
  TSTNode<T>* insertright(const T& value);
  TSTNode<T>* insertmid(const T& value);

  int height() const { return height_; }
  int changeheight(int h) { return height_ = h; }
  TSTNode<T>*& left() { return left_; }
  TSTNode<T>*& right() { return right_; }
  TSTNode<T>*& mid() { return mid_; }
  TSTNode<T>*& parent() { return parent_; }
  T& value() { return value_; }

  bool operator< (const TSTNode<T>& tn) const { return value_ < tn.value; }
  bool operator> (const TSTNode<T>& tn) const { return value_ > tn.value; }
  bool operator== (const TSTNode<T>& tn) const { return value_ == tn.value; }

private:
  T value_;
  TSTNode<T>* left_;
  TSTNode<T>* right_;
  TSTNode<T>* mid_;
  TSTNode<T>* parent_;
  int height_;
};

template <typename T> TSTNode<T>* TSTNode<T>::insertleft(const T& value) {
  return left_ = new TSTNode(value, this);
}

template <typename T> TSTNode<T>* TSTNode<T>::insertright(const T& value) {
  return right_ = new TSTNode(value, this);
}

template <typename T> TSTNode<T>* TSTNode<T>::insertmid(const T& value) {
  return mid_ = new TSTNode(value, this);
}

#endif //TSTNODE_S
