#include <CPSViewI.h>

PSViewPackedArrayToken::
PSViewPackedArrayToken(CPSView *psview, PSVinteger max_length) :
 PSViewToken  (psview, PSVIEW_TOKEN_TYPE_PACKED_ARRAY,
               PSVIEW_TOKEN_COMPOSITE, PSVIEW_TOKEN_LITERAL,
               PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 packed_array_()
{
  packed_array_ = new PSViewPackedArray(max_length);
}

PSViewPackedArrayToken::
PSViewPackedArrayToken(const PSViewPackedArrayToken &packed_array_token) :
 PSViewToken  (packed_array_token),
 packed_array_()
{
  packed_array_ = new PSViewPackedArray(*packed_array_token.packed_array_);
}

PSViewPackedArrayToken::
~PSViewPackedArrayToken()
{
}

PSViewPackedArrayToken *
PSViewPackedArrayToken::
dup() const
{
  return new PSViewPackedArrayToken(*this);
}

int
PSViewPackedArrayToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    PSViewPackedArrayToken *packed_array_token = dynamic_cast<PSViewPackedArrayToken *>(token);

    return packed_array_->compare(packed_array_token->packed_array_);
  }
  else
    return type_ - token->getType();
}

PSVinteger
PSViewPackedArrayToken::
getNumValues()
{
  return packed_array_->getNumValues();
}

PSViewToken *
PSViewPackedArrayToken::
getValue(int i)
{
  return packed_array_->getValue(i);
}

void
PSViewPackedArrayToken::
setValue(int i, PSViewToken *token)
{
  packed_array_->setValue(i, token);
}

void
PSViewPackedArrayToken::
executeToken()
{
  psview_->getOperandStack()->push(this);
}

const PSViewName &
PSViewPackedArrayToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewPackedArrayToken::
print()
{
  CStrUtil::printf("[");

  for (int i = 1; i <= packed_array_->getNumValues(); i++) {
    if (i > 1)
      CStrUtil::printf(" ");

    PSViewToken *token = packed_array_->getValue(i);

    token->print();
  }

  CStrUtil::printf("]");
}

string
PSViewPackedArrayToken::
toString()
{
  return "--nostringval--";
}
