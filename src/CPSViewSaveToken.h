class PSViewSaveToken : public PSViewToken {
 private:
  CAutoPtr<PSViewSave> save_;

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
};
