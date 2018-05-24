#include "Word.h"
#include <string>
#include <iostream>
using std::cout;
using std::string;

/*--------------For Word --------------*/
Word& Word::operator= (const Word& word) {
  if (this == &word)
    return *this;
  else {
    string::operator= (word);
    freq_ = word.freq();
    length_ = word.length();
    catagory_ = word.catagory();
    return *this;
  }
}

Word::Word(const string& str, double freq, const string& catagory): string(str), freq_(freq), length_(0), catagory_(catagory) {
  for (int i = 0; i < str.size(); ) {
    wordPoint_.push_back(i);
    if (!isCJK(i)) {
      if (!isLetter(i)) {
        i++;
      }
      else {
        while (i < str.size() && !isCJK(i) && isLetter(i)) {
          i++;
        }
      }
      length_++;
    }
    else {
      i += 2;
      length_++;
    }
  }
  wordPoint_.push_back(str.size());
}

Word::Word(const Word& word): string(word), freq_(word.freq()), length_(word.length()), catagory_(word.catagory()) {

}

/*-----For Character*/
Character& Character::operator= (const Character& ch) {
  if (this == &ch)
    return *this;
  else {
    Word::operator= (ch);
    setend(ch.freq_as_end(), ch.catagory_as_end());
    return *this;
  }
}
