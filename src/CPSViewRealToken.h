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

  PSViewRealToken *dup() const override;

  int compare(PSViewToken *token) override;

  void executeToken() override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  //----

  PSVreal getValue() const { return value_; }

 private:
  PSVreal value_ { 0.0 };
};

#endif
