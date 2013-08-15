#include "CPSViewI.h"

PSViewString::
PSViewString(PSVinteger max_length) :
 mem_()
{
  mem_ = new PSViewSharedChar(max_length);
}

PSViewString::
PSViewString(const string &str) :
 mem_()
{
  mem_ = new PSViewSharedChar(str.size());

  setString(str);
}

PSViewString::
PSViewString(const PSViewString &str, PSVinteger pos) :
 mem_()
{
  mem_ = new PSViewSharedChar(str.mem_, pos - 1);
}

PSViewString::
PSViewString(const PSViewString &str, PSVinteger pos, PSVinteger len) :
 mem_()
{
  mem_ = new PSViewSharedChar(str.mem_, pos - 1, len);
}

PSViewString::
~PSViewString()
{
}

string
PSViewString::
getString() const
{
  const char *data = mem_->getData();

  return string(data, mem_->getLen());
}

int
PSViewString::
getLength() const
{
  return mem_->getLen();
}

int
PSViewString::
getChar(int pos) const
{
  return mem_->getValue(pos - 1);
}

void
PSViewString::
setString(const string &str)
{
  int len = str.size();

  mem_->setData((char *) str.c_str(), len);

  mem_->setBounds(0, len);
}

void
PSViewString::
setChar(int pos, char c)
{
  mem_->setValue(pos - 1, c);
}

void
PSViewString::
setChars(const string &str, int pos)
{
  mem_->setData(pos - 1, (char *) str.c_str(), str.size());
}

void
PSViewString::
setBounds(int pos, int len)
{
  mem_->setBounds(pos - 1, len);
}

PSVinteger
PSViewString::
isSubString(PSViewString *str)
{
  if (str->mem_->getLen() > mem_->getLen())
    return -1;

  for (int i = 0; i <= mem_->getLen() - str->mem_->getLen(); i++) {
    if (getChar(mem_->getStart() + i + 1) == str->getChar(1)) {
      int j = 1;

      for ( ; j < str->mem_->getLen(); j++) {
        if (getChar(mem_->getStart() + i + j + 1) != str->getChar(str->mem_->getStart() + j + 1))
          break;
      }

      if (j == str->mem_->getLen())
        return (i + 1);
    }
  }

  return -1;
}

PSViewString *
PSViewString::
subString(PSVinteger start, PSVinteger num_chars)
{
  return new PSViewString(*this, start, num_chars);
}

PSViewString *
PSViewString::
split(int n)
{
  PSViewString *str = new PSViewString(*this, n + 1);

  setBounds(1, n);

  return str;
}

int
operator==(const PSViewString &str1, const PSViewString &str2)
{
  return str1.compare(str2);
}

int
PSViewString::
compare(const PSViewString &str) const
{
  PSVinteger i = mem_->getStart();
  PSVinteger j = str.mem_->getStart();
  PSVinteger k = 0;

  for ( ; k < mem_->getLen() && k < str.mem_->getLen(); i++, j++, k++) {
    if (getChar(i + 1) != str.getChar(j + 1))
      break;
  }

  char c1 = '\0';
  char c2 = '\0';

  if (k < mem_->getLen())
    c1 = getChar(i + 1);

  if (k < str.mem_->getLen())
    c2 = str.getChar(j + 1);

  return (int) (c1 - c2);
}

int
PSViewString::
compareN(const PSViewString &str, PSVinteger n) const
{
  PSVinteger i = mem_->getStart();
  PSVinteger j = str.mem_->getStart();
  PSVinteger k = 0;

  for ( ; k < mem_->getLen() && k < str.mem_->getLen() && k < n;
          i++, j++, k++) {
    if (getChar(i + 1) != str.getChar(j + 1))
      break;
  }

  char c1 = '\0';
  char c2 = '\0';

  if (k < n && k < mem_->getLen())
    c1 = getChar(i + 1);

  if (k < n && k < str.mem_->getLen())
    c2 = str.getChar(j + 1);

  return (int) (c1 - c2);
}
