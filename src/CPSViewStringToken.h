#ifndef CPSViewStringToken_H
#define CPSViewStringToken_H

class PSViewStringToken : public PSViewToken {
 public:
  PSViewStringToken(CPSView *psview, PSVinteger max_length);
  PSViewStringToken(CPSView *psview, const std::string &str);
  PSViewStringToken(CPSView *psview, PSViewString *str);
  PSViewStringToken(const PSViewStringToken &string_token);
  PSViewStringToken(const PSViewStringToken &string_token, int len);
  PSViewStringToken(const PSViewStringToken &string_token, int pos, int len);

  //----

  // virtual functions

 ~PSViewStringToken();

  PSViewStringToken *dup() const override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  void executeToken() override;

  int compare(PSViewToken *token) override;

  //----

  PSViewStringToken *split(PSVinteger pos);

  int compare(PSViewToken *token) const;

  int compareN(const PSViewStringToken *str, PSVinteger n) const;

  PSVinteger isSubString(PSViewStringToken *token);

  PSViewStringToken *subString(PSVinteger start, PSVinteger num_chars);

  void setString(const std::string &str);

  void setChar(int pos, char c);

  void setChars(const std::string &str, int pos);

  void setBounds(PSVinteger pos, PSVinteger len);

  int getLength() const;

  int getChar(int i) const;

 private:
  CAutoPtr<PSViewString> string_;
};

#endif
