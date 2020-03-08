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

  PSViewBooleanToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();

  //----

  PSVboolean getValue() const { return value_; }
};

#endif
