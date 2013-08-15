#ifndef PSVIEW_STRING_H
#define PSVIEW_STRING_H

class PSViewString {
  typedef CSharedMem<char> PSViewSharedChar;

 private:
  CAutoPtr<PSViewSharedChar> mem_;

 public:
  PSViewString(PSVinteger max_length);
  PSViewString(const std::string &str);
  PSViewString(const PSViewString &str, PSVinteger start);
  PSViewString(const PSViewString &str, PSVinteger start, PSVinteger len);
 ~PSViewString();

  std::string getString() const;

  int getLength() const;

  int getChar(int i) const;

  void setString(const std::string &str);
  void setChar(int pos, char c);
  void setChars(const std::string &str, int pos);

  void setBounds(int pos, int len);

  PSVinteger isSubString(PSViewString *str);

  PSViewString *subString(PSVinteger start, PSVinteger num_chars);

  PSViewString *split(int pos);

  int compare(const PSViewString &str) const;
  int compareN(const PSViewString &str, PSVinteger n) const;

  friend int operator==(const PSViewString &string1, const PSViewString &string2);
};

#endif
