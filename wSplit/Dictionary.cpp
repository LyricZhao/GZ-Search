#include <string>
#include "Word.h"
#include "TST.h"
#include "TSTNode.h"
#include <list>
#include <fstream>
#include <cstring>
#include "Dictionary.h"
#include <cmath>
using std::string;
using std::list;
using std::ifstream;
using std::ofstream;

const double Dict::NONELOGFREQ = (double)10;

Dict::Dict(const char* FileDirection): chend_(false), totalWord_(0), opened_(false) {
  FileDirection_ = new char[strlen(FileDirection) + 1];
  strcpy(FileDirection_, FileDirection);
}

Dict::Dict(): totalWord_(0), opened_(false) {
  FileDirection_ = nullptr;
}

Dict::~Dict() {
  delete[] FileDirection_;
}

bool Dict::load() {
  fin.open(FileDirection_);
  if (!fin) {
    cout << "Word Library open failed.\n";
    return false;
  }
  string temp;
  double freq = 0;
  string catagory;
  fin >> totalFreq_;
  double logfreq = log(totalFreq_);
  while(fin >> temp >> freq >> catagory) {
    insertword(Word(temp, -log(freq) + logfreq, catagory));
    totalWord_++;
  }
  // cout << "Dictionary load complete. " << totalWord_ << " words in all.\n";
  fin.close();
  return opened_ = true;
}

bool Dict::update() {
  return false;
}

bool Dict::isopen() const {
  return opened_;
}

bool Dict::getmatch(const string &text){
  TSTNode<Character>* find = (TSTNode<Character>*) nullptr;
  // std:: cout << "dic 00" << std:: endl;
  // std:: cout << text << std:: endl;
  // std:: cout << text.size() << std:: endl;
  find = searchword(text);
  // std:: cout << "e ok" << std:: endl;
  if ((find = searchword(text)) && chend_) {
    // std:: cout << "dic 01" << std:: endl;
    findResult_ = Word(text, (find -> value()).freq_as_end(), (find -> value()).catagory_as_end());
    return true;
  }
  else {
    // std:: cout << "dic 02" << std:: endl;
    findResult_ = Word(text, NONELOGFREQ);
  }
  return false;
}

TSTNode<Character>* Dict::insertchar(const Character& ch) {
  return nullptr;
}

TSTNode<Character>* Dict::insertword(const Word& word) { // transfome to the real pointer before return
  TSTNode<Character>** cur_root = &ttree_.root(); //a pointer to the pointer of node root, make it possible to do with the directly new root, instead of a copy
  TSTNode<Character>* sear = *cur_root;   //not need for sear to be reference, a copy of the new root
  int offset = 0;
  while(offset < word.length()) {
    sear = ttree_.insert(word[offset], *cur_root); // sear ether point to the inserted new node, or to the existed node
    cur_root = &(sear -> mid());
    offset++;
  }
  (sear -> value()).setend(word.freq(), word.catagory());
  return sear;
}

TSTNode<Character>* Dict::searchchar(const Character& ch, TSTNode<Character>*& begin) {
  return ttree_.search(ch, begin);
}

TSTNode<Character>* Dict::searchword(const Word& word) {
  // std:: cout << "ok" << std:: endl;
  TSTNode<Character>* cur_root = ttree_.root();
  chend_ = false;
  TSTNode<Character>* sear = cur_root;
  int offset = 0;
  // std:: cout << "ok" << std:: endl;
  while (offset < word.length()) {
    // std:: cout << "ok" << std:: endl;
    // std:: cout << word[offset] << std:: endl;
    sear = searchchar(word[offset], cur_root);
    if (!sear) return sear;
    else {
      chend_ = (sear -> value()).end();
      cur_root = sear -> mid();
    }
    offset++;
  }
  return sear;
}

void Dict::debug() {
  //cout << (ttree_.root()) -> value() << (ttree_.root()) -> right() -> value() << " rightvalue\n";
  //cout << (searchword(Word("����")) -> parent()) << " midparent\n";
}
