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

  PSViewIntegerToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();

  //----

  PSVinteger getValue() const { return value_; }

 private:
  PSVinteger value_ { 0 };
};

#endif
