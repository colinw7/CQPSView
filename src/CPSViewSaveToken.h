#ifndef CPSViewSaveToken_H
#define CPSViewSaveToken_H

class PSViewSaveToken : public PSViewToken {
 public:
  PSViewSaveToken(CPSView *psview, void *data);
  PSViewSaveToken(const PSViewSaveToken &save_token);

  //----

  // virtual functions

  ~PSViewSaveToken();

  PSViewSaveToken *dup() const;

  const PSViewName &getName();

  void print();

  std::string toString();

  void executeToken();

  int compare(PSViewToken *token);

  //----

  void *getValue();

 private:
  CAutoPtr<PSViewSave> save_;
};

#endif
