#ifndef CJKTOKENIZER_
#define CJKTOKENIZER_

#include "Word.h"
#include "Dictionary.h"
#include "WordGraph.h"

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::vector;
using std::string;
using std::pair;

class CJKTokenizer {
public:
  CJKTokenizer();
  CJKTokenizer(const char* filename = nullptr, const char* invalidWordFile = nullptr);

  void clear () { token_.clear(); }
  void tokenize(const string& text);
  void showToken() const {
    for (int i = 0 ; i < token_.size(); i++) {
      cout << token_[i] << "/";
    }
  }

  vector<string> token_; //the result for tokenizing
  void split2word(std::string text, vector< pair<string, int> >& v);
private:
  Dict dict_;
  const char* invalidWordFile_;
  vector<string> invalidWord;
  void invalidWordLoad(const char* invalidWordFile);
  bool validWord(const string& word);
  void maxLengthFit(const Word& text);
  void wordGraphDijk(const Word& text);
};

#endif //CJKTOKENIZER_
