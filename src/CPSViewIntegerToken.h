#ifndef CPSViewIntegerToken_H
#define CPSViewIntegerToken_H

class PSViewRealToken;

class PSViewIntegerToken : public PSViewToken {
 public:
  PSViewIntegerToken(CPSView *psview, PSVinteger value);
  PSViewIntegerToken(const PSViewIntegerToken &integer_token);

  //----

  // virtual functions

  ~PSViewIntegerToken();

  PSViewIntegerToken *dup() const override;

  int compare(PSViewToken *token) override;

  void executeToken() override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  //----

  PSVinteger getValue() const { return value_; }

 private:
  PSVinteger value_ { 0 };
};

#endif
