class PSViewNullToken : public PSViewToken {
 public:
  PSViewNullToken(CPSView *psview);

  PSViewNullToken(const PSViewNullToken &null_token);

  //----

  // virtual functions

  ~PSViewNullToken();

  PSViewNullToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();
};
