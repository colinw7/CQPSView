class PSViewMarkToken : public PSViewToken {
 public:
  PSViewMarkToken(CPSView *psview);

  // virtual functions

 ~PSViewMarkToken();

  PSViewMarkToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();
};
