#ifndef CPSViewBooleanToken_H
#define CPSViewBooleanToken_H

class PSViewBooleanToken : public PSViewToken {
 private:
  PSVboolean value_;

 public:
  PSViewBooleanToken(CPSView *psview, PSVboolean value);
  PSViewBooleanToken(const PSViewBooleanToken &boolean_token);

  //----

  // virtual functions

 ~PSViewBooleanToken();

  PSViewBooleanToken *dup() const override;

  int compare(PSViewToken *token) override;

  void executeToken() override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  //----

  PSVboolean getValue() const { return value_; }
};

#endif
