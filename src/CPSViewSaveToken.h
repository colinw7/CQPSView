#ifndef CPSViewSaveToken_H
#define CPSViewSaveToken_H

class PSViewSaveToken : public PSViewToken {
 public:
  PSViewSaveToken(CPSView *psview, void *data);
  PSViewSaveToken(const PSViewSaveToken &save_token);

  //----

  // virtual functions

  ~PSViewSaveToken();

  PSViewSaveToken *dup() const override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  void executeToken() override;

  int compare(PSViewToken *token) override;

  //----

  void *getValue();

 private:
  CAutoPtr<PSViewSave> save_;
};

#endif
