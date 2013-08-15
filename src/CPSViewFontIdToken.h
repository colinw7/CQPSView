class PSViewFontIdToken : public PSViewToken {
 private:
  const PSVfont_id value_;

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
};
