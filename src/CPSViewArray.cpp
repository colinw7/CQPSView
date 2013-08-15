#include "CPSViewI.h"

PSViewArray::
PSViewArray(PSVinteger max_length) :
 tokens_()
{
  tokens_ = new PSViewSharedToken(max_length);
}

PSViewArray::
PSViewArray(const PSViewArray &array) :
 tokens_()
{
  tokens_ = new PSViewSharedToken(array.tokens_);
}

PSViewArray::
PSViewArray(const PSViewArray &array, PSVinteger n) :
 tokens_()
{
  tokens_ = new PSViewSharedToken(array.tokens_, n - 1);
}

PSViewArray::
PSViewArray(const PSViewArray &array, PSVinteger pos, PSVinteger len) :
 tokens_()
{
  tokens_ = new PSViewSharedToken(array.tokens_, pos - 1, len);
}

PSViewArray::
~PSViewArray()
{
}

PSViewArray *
PSViewArray::
dup() const
{
  return new PSViewArray(*this);
}

int
PSViewArray::
compare(PSViewArray *array)
{
  PSVinteger num_values1 = getNumValues();
  PSVinteger num_values2 = array->getNumValues();

  if      (num_values1 > num_values2)
    return  1;
  else if (num_values1 < num_values2)
    return -1;

  for (PSVinteger i = 1; i <= num_values1; ++i) {
    PSViewToken *token1 = getValue(i);
    PSViewToken *token2 = array->getValue(i);

    int cmp = token1->compare(token2);

    if (cmp != 0)
      return cmp;
  }

  return 0;
}

PSVinteger
PSViewArray::
getNumValues()
{
  return tokens_->getLen();
}

PSViewToken *
PSViewArray::
getValue(uint i)
{
  return tokens_->getValue(i - 1);
}

void
PSViewArray::
setValue(uint i, PSViewToken *value)
{
  tokens_->setValue(i - 1, value);
}

void
PSViewArray::
setSubValues(uint pos, PSViewToken **sub_tokens, uint num_sub_tokens)
{
  // getPSView()->getMemoryMgr()->change((char **) &array->tokens);

  for (uint i = 0; i < num_sub_tokens; ++i)
    setValue(pos + i, sub_tokens[i]);
}

PSViewArray *
PSViewArray::
split(uint n)
{
  PSViewArray *array = new PSViewArray(*this, n + 1);

  setBounds(1, n);

  return array;
}

void
PSViewArray::
setBounds(int pos, uint len)
{
  if (pos == -1)
    tokens_->setBounds(0, 0);
  else
    tokens_->setBounds(pos - 1, len);
}
