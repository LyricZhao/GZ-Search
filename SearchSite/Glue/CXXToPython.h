#ifndef CXXTOPYTHON_H_
#define CXXTOPYTHON_H_

#include "context.h"
#include "requestCore.h"

#include <vector>
#include <string>
#include <iostream>

class CXXToPython {
private:
  requestCore m_requestCore;
  std::string m_keyWord;
  std::vector<userCONTEXT> m_result;
  bool m_ifinit;
public:
  CXXToPython() {
    m_ifinit = false;
  }
  ~CXXToPython() {}
  void init() { m_requestCore.init(); m_ifinit = true; }
  bool ifinit() const { return m_ifinit; }
  void vend() { m_requestCore.vend(); }
  void CXXSearch(std::string keyword);
  const char* GetTitle(int n);
  const char* GetUrl(int n);
  const char* GetContext(int n);
  int GetType(int n)  { return m_result[n].type; }//0: news, 1: answer
  int ResultSize()  { return m_result.size(); }

  void CreateInstance(int k);
};

#endif //CXXTOPYTHON_H_
