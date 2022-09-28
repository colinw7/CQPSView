#include <CPSViewI.h>

PSViewPackedArray::
PSViewPackedArray(PSVinteger max_length) :
 tokens_()
{
  tokens_ = new PSViewSharedToken(int(max_length));
}

PSViewPackedArray::
PSViewPackedArray(const PSViewPackedArray &array) :
 tokens_()
{
  tokens_ = new PSViewSharedToken(array.tokens_);
}

PSViewPackedArray::
PSViewPackedArray(const PSViewPackedArray &array, PSVinteger n) :
 tokens_()
{
  tokens_ = new PSViewSharedToken(array.tokens_, int(n));
}

PSViewPackedArray::
~PSViewPackedArray()
{
}

int
PSViewPackedArray::
compare(PSViewPackedArray *array)
{
  int num_values1 = int(       getNumValues());
  int num_values2 = int(array->getNumValues());

  if      (num_values1 > num_values2)
    return  1;
  else if (num_values1 < num_values2)
    return -1;

  for (int i = 1; i <= num_values1; ++i) {
    PSViewToken *token1 = getValue(i);
    PSViewToken *token2 = array->getValue(i);

    int cmp = token1->compare(token2);

    if (cmp != 0)
      return cmp;
  }

  return 0;
}

PSVinteger
PSViewPackedArray::
getNumValues()
{
  return tokens_->getLen();
}

PSViewToken *
PSViewPackedArray::
getValue(int i)
{
  return tokens_->getValue(i - 1);
}

void
PSViewPackedArray::
setValue(int i, PSViewToken *value)
{
  tokens_->setValue(i - 1, value);
}
