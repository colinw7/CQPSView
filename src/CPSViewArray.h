#ifndef CPSViewArray_H
#define CPSViewArray_H

class PSViewArray {
  typedef CSharedMem<PSViewToken *> PSViewSharedToken;

 public:
  PSViewArray(PSVinteger max_length);
  PSViewArray(const PSViewArray &array);
  PSViewArray(const PSViewArray &array, PSVinteger pos);
  PSViewArray(const PSViewArray &array, PSVinteger pos, PSVinteger len);
 ~PSViewArray();

  PSViewArray *dup() const;

  int compare(PSViewArray *array);

  PSVinteger   getNumValues();
  PSViewToken *getValue(uint i);
  void         setValue(uint i, PSViewToken *token);

  void setSubValues(uint pos, PSViewToken **sub_tokens, uint num_sub_tokens);

  PSViewArray *split(uint n);

  void setBounds(int pos, uint len);

 private:
  PSViewArray();

  PSViewArray &operator=(const PSViewArray &array);

 private:
  CAutoPtr<PSViewSharedToken> tokens_;
};

#endif
