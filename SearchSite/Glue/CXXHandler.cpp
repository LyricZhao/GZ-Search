#include "CXXToPython.h"
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>

std::string PyStrToCXXStr(const wchar_t* str, int len) {
    std::wstring ws(str);

    //setup converter
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    std::string converted_str = converter.to_bytes( ws );
    return converted_str;
}

extern "C" {
  CXXToPython cxxToPython;

  void CreateInstance() {
    cxxToPython.CreateInstance(1);
  }

  void init() {
    cxxToPython.init();
  }

  void vend() { cxxToPython.vend(); }

  void CXXSearch(const wchar_t* keyword, int len) {
    cxxToPython.CXXSearch(PyStrToCXXStr(keyword, len));
  }

  const char* GetTitle(int n) {
    return cxxToPython.GetTitle(n);
  }

  const char* GetUrl(int n) {
    return cxxToPython.GetUrl(n);
  }

  const char* GetContext(int n) {
    return cxxToPython.GetContext(n);
  }

  const char* ShowInput(const wchar_t* input, int len) {
    std::string cxxstr = PyStrToCXXStr(input, len);
    return cxxstr.c_str();
  }

  int GetType(int n) {
    return cxxToPython.GetType(n);
  }//0: news, 1: answer

  int ResultSize() {
    return cxxToPython.ResultSize();
  }

  bool ifinit() {
    return cxxToPython.ifinit();
  }
}
