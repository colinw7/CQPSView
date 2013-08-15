class PSViewIntegerToken;

class PSViewRealToken : public PSViewToken {
 private:
  PSVreal value_;

 public:
  PSViewRealToken(CPSView *psview, PSVreal value);
  PSViewRealToken(const PSViewRealToken &real_token);

  //----

  // virtual functions

  ~PSViewRealToken();

  PSViewRealToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();

  //----

  PSVreal getValue() const { return value_; }
};
