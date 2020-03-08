#ifndef CPSViewPackedArray_H
#define CPSViewPackedArray_H

class PSViewPackedArray {
 public:
  PSViewPackedArray(PSVinteger max_length);
  PSViewPackedArray(const PSViewPackedArray &array);
  PSViewPackedArray(const PSViewPackedArray &array, PSVinteger n);
 ~PSViewPackedArray();

  int compare(PSViewPackedArray *array);

  PSVinteger   getNumValues();
  PSViewToken *getValue(int i);
  void         setValue(int i, PSViewToken *token);

 private:
  typedef CSharedMem<PSViewToken *> PSViewSharedToken;

  CAutoPtr<PSViewSharedToken> tokens_;
};

#endif
