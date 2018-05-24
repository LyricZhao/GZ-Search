#ifndef DICTIONARY_
#define DICTIONARY_
#include <string>
#include "Word.h"
#include <list>
#include "TST.h"
#include "TSTNode.h"
#include <fstream>
using std::string;
using std::list;
using std::ifstream;
using std::ofstream;

#define nullptr 0

class Dict {
public:
  Dict(const char* FileDirection);
  Dict();
  ~Dict();
  bool load();
  bool update();
  bool getmatch(const string &text);
  bool isopen() const;
  TSTNode<Character>* insertchar(const Character& ch);
  TSTNode<Character>* insertword(const Word& word); //return the pointer of the end of the inserted word
  TSTNode<Character>* searchchar(const Character& ch, TSTNode<Character>*& begin) ;
  TSTNode<Character>* searchword(const Word& word); // return the pointer of the end of the word
  void debug();
  const Word& findResult() const { return findResult_; }
  long long totalWord() const { return totalWord_; }
  double totalFreq() const { return totalFreq_; }

private:
  TST<Character> ttree_;
  long long totalWord_;
  bool chend_; //if the place search() terminated is the end of a word
  double totalFreq_; // the freqent number of all words
  Word findResult_;
  char* FileDirection_;
  list<Word> WordMatch;
  list<Word> it;
  bool opened_;
  ifstream fin;
  ofstream fout;

  static const double NONELOGFREQ; //log freq for characters that doesn't exist
};

#endif //DICTIONARY_
