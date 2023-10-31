#ifndef CPSViewArrayToken_H
#define CPSViewArrayToken_H

class PSViewArrayToken : public PSViewToken {
 public:
  PSViewArrayToken(CPSView *psview, uint max_length);
  PSViewArrayToken(CPSView *psview, CMatrix2D *matrix);
  PSViewArrayToken(const PSViewArrayToken &array_token);
  PSViewArrayToken(const PSViewArrayToken &array_token, uint n);
  PSViewArrayToken(CPSView *psview, PSViewArray *array);

  //----

  // virtual functions

 ~PSViewArrayToken();

  PSViewArrayToken *dup() const override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  void executeToken() override;

  int compare(PSViewToken *token) override;

  //----

  PSVinteger   getNumValues();
  PSViewToken *getValue(uint i);
  void         setValue(uint i, PSViewToken *token);

  void setSubValues(PSVinteger pos, PSViewToken **sub_tokens, PSVinteger num_sub_tokens);

  PSViewArrayToken *split(PSVinteger n);

  void setBounds(PSVinteger pos, PSVinteger len);

 private:
  PSViewArrayToken &operator=(const PSViewArrayToken &array_token);

 private:
  CAutoPtr<PSViewArray> array_;
};

#endif
