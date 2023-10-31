#ifndef CPSViewFontIdToken_H
#define CPSViewFontIdToken_H

class PSViewFontIdToken : public PSViewToken {
 public:
  PSViewFontIdToken(CPSView *psview, PSVfont_id value);
  PSViewFontIdToken(const PSViewFontIdToken &font_id_token);

  //----

  // virtual functions

  ~PSViewFontIdToken();

  PSViewFontIdToken *dup() const override;

  int compare(PSViewToken *token) override;

  void executeToken() override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

 private:
  const PSVfont_id value_ { 0 };
};

#endif
