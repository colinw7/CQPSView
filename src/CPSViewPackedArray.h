class PSViewPackedArray {
  typedef CSharedMem<PSViewToken *> PSViewSharedToken;

 private:
  CAutoPtr<PSViewSharedToken> tokens_;

 public:
  PSViewPackedArray(PSVinteger max_length);
  PSViewPackedArray(const PSViewPackedArray &array);
  PSViewPackedArray(const PSViewPackedArray &array, PSVinteger n);
 ~PSViewPackedArray();

  int compare(PSViewPackedArray *array);

  PSVinteger   getNumValues();
  PSViewToken *getValue(int i);
  void         setValue(int i, PSViewToken *token);
};
