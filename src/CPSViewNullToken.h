#ifndef CPSViewNullToken_H
#define CPSViewNullToken_H

class PSViewNullToken : public PSViewToken {
 public:
  PSViewNullToken(CPSView *psview);

  PSViewNullToken(const PSViewNullToken &null_token);

  //----

  // virtual functions

  ~PSViewNullToken();

  PSViewNullToken *dup() const override;

  int compare(PSViewToken *token) override;

  void executeToken() override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;
};

#endif
