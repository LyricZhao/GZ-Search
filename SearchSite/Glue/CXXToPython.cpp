#include "context.h"
#include "CXXToPython.h"

#include <string>
#include <vector>
#include <assert.h>

void CXXToPython::CXXSearch(std::string keyword) {
  m_result.clear();
  m_keyWord = keyword;
  m_requestCore.search(keyword, m_result);
}

const char* CXXToPython::GetTitle(int n) {
  assert(n < m_result.size());
  return m_result[n].title.c_str();
}

const char* CXXToPython::GetUrl(int n) {
  assert(n < m_result.size());
  return m_result[n].url.c_str();
}

const char* CXXToPython::GetContext(int n) {
  assert(n < m_result.size());
  return m_result[n].context.c_str();
}

void CXXToPython::CreateInstance(int k) {
  userCONTEXT uc;
  uc.type = 1;
  uc.title = "刘欣是我的儿子";
  uc.url = "http://www.baidu.com";
  uc.context = "啦啦啦啦啦啦啦啦啦啦啦啦啦啦啦啦啦啦啦";
  m_result.push_back(uc);
}
