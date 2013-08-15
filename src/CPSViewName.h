#ifndef PSViewName_H
#define PSViewName_H

#include <map>

class PSViewNameMgr {
 private:
  typedef std::map<std::string,PSViewName *> NameMap;

  CPSView *psview_;
  NameMap  names_;

 public:
  PSViewNameMgr(CPSView *psview);

  const PSViewName &getName(const std::string &str);

 private:
  PSViewNameMgr(const PSViewNameMgr &rhs);
  PSViewNameMgr &operator=(const PSViewNameMgr &rhs);
};

class PSViewName {
 private:
  std::string str_;

 public:
  PSViewName(const std::string &str);
 ~PSViewName();

  int compare(const PSViewName &name) const;

  const std::string &getString() const { return str_; }
  int                getLength() const { return str_.size(); }
};

#endif
