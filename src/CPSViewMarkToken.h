#ifndef CPSViewMarkToken_H
#define CPSViewMarkToken_H

class PSViewMarkToken : public PSViewToken {
 public:
  PSViewMarkToken(CPSView *psview);

  // virtual functions

 ~PSViewMarkToken();

  PSViewMarkToken *dup() const override;

  int compare(PSViewToken *token) override;

  void executeToken() override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;
};

#endif
