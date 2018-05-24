#include "CJKTokenizer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include "Dictionary.h"
#include "Word.h"
#include <map>
#include "convert.h"
using std::vector;
using std::string;
using std::cout;
using std::stack;
using std::pair;
using std::map;

CJKTokenizer::CJKTokenizer() {}

CJKTokenizer::CJKTokenizer(const char* filename, const char* invalidWordFile )
          :dict_(filename), invalidWordFile_(invalidWordFile)
{
  dict_.load();
  invalidWordLoad(invalidWordFile);
}

void CJKTokenizer::invalidWordLoad(const char* invalidWordFile) {
  std::ifstream fin(invalidWordFile_);
  if (fin) {
    string word;
    while ((fin >> word)) {
      invalidWord.push_back(word);
    }
  }
  invalidWord.push_back(" ");
  invalidWord.push_back("　");
  invalidWord.push_back("\n");
  invalidWord.push_back("\t"); //special characters
  fin.close();
}

void CJKTokenizer::tokenize(const string& text) {
  clear();
  if (!dict_.isopen()) {
    cout << "the dictionary is not open.\n";
    return;
  }
// maxLengthFit(Word(text));
  wordGraphDijk(text);
}

void CJKTokenizer::maxLengthFit(const Word& text) {
  int offset_begin = 0;
  int max_end = 0;   // more that one character
  int count = 0;
  while (max_end <= text.length()) {
    int offset_end = max_end;
    while (offset_end <= text.length()) {
      cout << text.subWord(offset_begin, offset_end - offset_begin) << "\n";
      if (dict_.getmatch(text.subWord(offset_begin, offset_end - offset_begin))) {

        max_end = offset_end;
      }
      offset_end++;
      count++;
    }// fit until the word doesn't exist
    if (max_end <= offset_begin + 1) {       // a character that is not in the dictionary
      token_.push_back(text.subWord(offset_begin, 1));
      dict_.insertword(text.subWord(offset_begin, 1));
      offset_begin++;
    }
    else {
      token_.push_back(dict_.findResult());
      offset_begin = max_end;
    }
    max_end = offset_begin + 1;
  }
}

void CJKTokenizer::wordGraphDijk(const Word& text) {
  // std:: cout << "run 01" << std:: endl;
  WordGraph wg(text, dict_);
  // std:: cout << "run 02" << std:: endl;
  wg.Dijkstra();
  stack<int> result;
  int ed = wg.fatherEdge()[text.length()], ver = text.length();
  while (ed != -1) {
    result.push(ed);
    ver = wg.edge(ed) -> pv();
    ed = wg.fatherEdge()[ver];
  }
  while (!result.empty()) {
    if (!validWord(wg.edge(result.top()) -> value())) {
      token_.push_back(wg.edge(result.top()) -> value());
    }
    result.pop();
  }
}

bool CJKTokenizer::validWord(const string& word) {
  for (int i = 0; i < invalidWord.size(); i++) {
    if (word == invalidWord[i]) return true;
  }
  return false;
}


void CJKTokenizer::split2word(std::string text, vector< pair<string, int> >& v) {
  text = utf82gbk(text);
  //std:: cout << "c1 here" << std:: endl;
  //std:: cout << text << std:: endl;
  //std:: cout << "c1 ok" << std:: endl;
  tokenize(text);
  //std:: cout << "c2 here" << std:: endl;
  map<string, int> tokenMap;
  for (int i = 0; i < token_.size(); i++) {
    tokenMap[token_[i]]++;
  }
  //std:: cout << "c3 here" << std:: endl;
  map<string, int>::iterator it = tokenMap.begin();
  while (it != tokenMap.end()) {
    //std:: cout << "processing***" << std:: endl;
    pair<string, int> tokenPair(gbk2utf8((it -> first).c_str()), it -> second);
    v.push_back(tokenPair);
    it++;
    //std:: cout << "ok" << std:: endl;
  }
  // std:: cout << "all done" << std:: endl;
}
