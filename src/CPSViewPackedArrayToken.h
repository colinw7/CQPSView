class PSViewPackedArrayToken : public PSViewToken {
 private:
  CAutoPtr<PSViewPackedArray> packed_array_;

 public:
  PSViewPackedArrayToken(CPSView *psview, PSVinteger max_length);
  PSViewPackedArrayToken(const PSViewPackedArrayToken &packed_array_token);

  //----

  // virtual functions

  ~PSViewPackedArrayToken();

  PSViewPackedArrayToken *dup() const;

  const PSViewName &getName();

  void print();

  std::string toString();

  void executeToken();

  int compare(PSViewToken *token);

  //----

  PSVinteger   getNumValues();
  PSViewToken *getValue(int i);
  void         setValue(int i, PSViewToken *token);
};
