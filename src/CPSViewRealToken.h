#ifndef CPSViewRealToken_H
#define CPSViewRealToken_H

class PSViewIntegerToken;

class PSViewRealToken : public PSViewToken {
 public:
  PSViewRealToken(CPSView *psview, PSVreal value);
  PSViewRealToken(const PSViewRealToken &real_token);

  //----

  // virtual functions

  ~PSViewRealToken();

  PSViewRealToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();

  //----

  PSVreal getValue() const { return value_; }

 private:
  PSVreal value_ { 0.0 };
};

#endif
