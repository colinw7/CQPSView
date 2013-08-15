class PSViewOperatorToken : public PSViewToken {
 private:
  PSViewOperator *operator_;

 public:
  PSViewOperatorToken(CPSView *psview, const std::string &name);
  PSViewOperatorToken(CPSView *psview, PSViewOperator *opr);
  PSViewOperatorToken(const PSViewOperatorToken &operator_token);

  //----

  // virtual functions

  ~PSViewOperatorToken();

  PSViewOperatorToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();

  //----

  PSViewOperator *getValue() const { return operator_; }

 private:
  PSViewOperatorToken &operator=(const PSViewOperatorToken &operator_token);
};
