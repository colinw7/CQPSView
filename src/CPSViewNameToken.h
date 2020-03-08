#ifndef CPSViewNameToken_H
#define CPSViewNameToken_H

class PSViewNameToken : public PSViewToken {
 public:
  PSViewNameToken(CPSView *psview, const PSViewName &value);
  PSViewNameToken(CPSView *psview, const std::string &str);
  PSViewNameToken(PSViewStringToken *token);
  PSViewNameToken(const PSViewNameToken &name_token);

  //----

  // virtual functions

  ~PSViewNameToken();

  PSViewNameToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();

  //----

  const PSViewName &getValue() const { return value_; }

  const std::string &getString();
  int                getLength();

 private:
  const PSViewName &value_;
};

#endif
