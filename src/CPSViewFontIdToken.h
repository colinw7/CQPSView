#ifndef CPSViewFontIdToken_H
#define CPSViewFontIdToken_H

class PSViewFontIdToken : public PSViewToken {
 public:
  PSViewFontIdToken(CPSView *psview, PSVfont_id value);
  PSViewFontIdToken(const PSViewFontIdToken &font_id_token);

  //----

  // virtual functions

  ~PSViewFontIdToken();

  PSViewFontIdToken *dup() const;

  int compare(PSViewToken *token);

  void executeToken();

  const PSViewName &getName();

  void print();

  std::string toString();

 private:
  const PSVfont_id value_ { 0 };
};

#endif
