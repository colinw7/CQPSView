#ifndef CPSViewName_H
#define CPSViewName_H

#include <map>

class PSViewNameMgr {
 public:
  PSViewNameMgr(CPSView *psview);

  const PSViewName &getName(const std::string &str);

 private:
  PSViewNameMgr(const PSViewNameMgr &rhs);
  PSViewNameMgr &operator=(const PSViewNameMgr &rhs);

 private:
  typedef std::map<std::string,PSViewName *> NameMap;

  CPSView *psview_;
  NameMap  names_;
};

//---

class PSViewName {
 public:
  PSViewName(const std::string &str);
 ~PSViewName();

  int compare(const PSViewName &name) const;

  const std::string &getString() const { return str_; }
  int                getLength() const { return str_.size(); }

 private:
  std::string str_;
};

#endif
