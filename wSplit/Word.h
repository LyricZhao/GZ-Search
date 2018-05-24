#ifndef WORD_
#define WORD_
#include <string>
#include <iostream>
#include <vector>
using std::cout;
using std::string;
using std::vector;

#define nullptr 0

class Word: public string {
public:
  Word(const string& str, double freq = 0, const string& catagory = "null");
  Word(const Word& word);
  Word(): string("\0"), freq_(0), length_(0), catagory_("null") {}

  Word& operator= (const Word& word);
  string operator[] (int x) const {
    return string::substr(wordPoint_[x], wordPoint_[x + 1] - wordPoint_[x]);
  /*  int offset = 0, i = 0;
    while ((i++) < x) isCJK(offset) ? offset += 2 : offset += 1;
    return isCJK(offset) ? string::substr(offset, 2) : string::substr(offset, 1);
    */
  }
  string subWord(int begin, int length) const {
    Word sub;
    for (int i = begin ; i < length + begin; i++) {
      sub = sub + this -> operator[] (i);
    }
    return sub;
  }

  double freq() const     { return freq_; }
  int length() const      { return length_; }
  string catagory() const { return catagory_; }

  /*check the category of charactor*/
  bool isLetter(int offset) const { return (string::at(offset) >= 'A' && string::at(offset) <= 'Z')
                                        || (string::at(offset) >= 'a' && string::at(offset) <= 'z')
                                        || (string::at(offset) >= '0' && string::at(offset) <= '9'); }
  bool isCJK(int offset) const {
    int siz = string:: size();
    return (offset <= siz - 2) && //more than two char
           ((unsigned char)string::at(offset) >= 0x81) && ((unsigned char)string::at(offset) <= 0xFE) && // first between 0x81 to 0xFE
           ((unsigned char)string::at(offset + 1) >= 0x40) && ((unsigned char)string::at(offset + 1) <= 0xFE); // second between 0x40 to 0xFE
  }

  /*--------------------pointer class----------------------------*/
  class Pointer {
  public:
    Pointer(Word& word, int pos = 0): offset_(pos) { word_ = &word; }
    Pointer(): word_(nullptr), offset_(0) {}

    void point_to(Word& word, int pos = 0) { word_ = &word; offset_ = pos; }
    /*overload of operator*/
    string operator*()          { return (word_ -> isCJK(offset_)) ? word_ -> substr(offset_, 2) : word_ -> substr(offset_, 1); }
    Pointer& operator++ ()      { (word_ -> isCJK(offset_)) ? offset_ += 2 : offset_ += 1; return *this; }
    Pointer& operator++ (int n) { (word_ -> isCJK(offset_)) ? offset_ += 2 : offset_ += 1; return *this; }
    Pointer& operator-- ()      { (word_ -> isCJK(offset_)) ? offset_ -= 2 : offset_ -= 1; return *this; }
    Pointer& operator-- (int n) { (word_ -> isCJK(offset_)) ? offset_ -= 2 : offset_ -= 1; return *this; }
    Pointer operator+ (int x)   {
      int offset = offset_, i = 0;
      while ((i++) < x) (word_ -> isCJK(offset)) ? offset += 2 : offset += 1;
      return Pointer(*word_, offset);
    }
    Pointer operator- (int x)   {
      int offset = offset_, i = 0;
      while ((i++) < x) (word_ -> isCJK(offset)) ? offset -= 2 : offset -= 1;
      return Pointer(*word_, offset);
    }
    /*seek or check the position of the pointer*/
    int seekend()   { return offset_ = word_ -> size(); }
    int seekbegin() { return offset_ = 0; }
    bool isbegin()  { return offset_ == 0; }
    bool isend()    { return offset_ == word_ -> size(); }
  private:
    Word* word_;
    int offset_;
  };
/*---------------------------Pointer class--------------------------*/

private:
  double freq_;          //-log(the freqent number) + log(total_freq)
  int length_;
  vector<int> wordPoint_; //the point between two words
  string catagory_;
};

class Character: public Word {
public:
  Character(const Word& ch, bool end = false, double freq_as_end = 0, const string& catagory_as_end = "null"):
            Word(ch), end_(end), freq_as_end_(freq_as_end), catagory_as_end_(catagory_as_end) {}
  Character(const string& str, double freq = 0, bool end = false, double freq_as_end = 0, string catagory_as_end = "null"):
            Word(str, freq), end_(end), freq_as_end_(freq_as_end), catagory_as_end_(catagory_as_end) {}
  Character(): Word(), end_(false) {}
  Character& operator= (const Character& ch);

  double freq_as_end() const      { return freq_as_end_; }
  string catagory_as_end() const  { return catagory_as_end_; }

  bool end() const { return end_; }
  void setend(double freq, const string& catagory) { freq_as_end_ = freq; end_ = true; catagory_as_end_ = catagory;}

private:
  bool end_;
  string catagory_as_end_;
  double freq_as_end_; //if the character is the end of a word, store the word's freq
};

#endif //WORD_
