#include <CPSViewI.h>

PSViewNameMgr::
PSViewNameMgr(CPSView *psview) :
 psview_(psview),
 names_ ()
{
}

const PSViewName &
PSViewNameMgr::
getName(const string &str)
{
  if (names_.find(str) != names_.end())
    return *names_[str];

  PSViewName *name = new PSViewName(str);

  names_[str] = name;

  return *name;
}

PSViewName::
PSViewName(const string &str) :
 str_(str)
{
}

PSViewName::
~PSViewName()
{
  CTHROW("Illegal Name deletion");
}

int
PSViewName::
compare(const PSViewName &name) const
{
  if      (str_ == name.str_)
    return 0;
  else if (str_ >  name.str_)
    return 1;
  else
    return -1;
}
