#include "CPSViewI.h"

PSViewArrayToken::
PSViewArrayToken(CPSView *psview, uint max_length) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_ARRAY,
             PSVIEW_TOKEN_COMPOSITE, PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 array_     ()
{
  array_ = new PSViewArray(max_length);
}

PSViewArrayToken::
PSViewArrayToken(CPSView *psview, CMatrix2D *matrix) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_ARRAY,
             PSVIEW_TOKEN_COMPOSITE, PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 array_     ()
{
  array_ = new PSViewArray(6);

  double data[6];

  matrix->getValues(&data[0], &data[2], &data[1],
                    &data[3], &data[4], &data[5]);

  for (uint i = 1; i <= 6; ++i) {
    PSViewToken *sub_token = new PSViewRealToken(psview_, data[i - 1]);

    setValue(i, sub_token);
  }
}

PSViewArrayToken::
PSViewArrayToken(const PSViewArrayToken &array_token) :
 PSViewToken(array_token),
 array_     ()
{
  array_ = array_token.array_->dup();
}

PSViewArrayToken::
PSViewArrayToken(const PSViewArrayToken &array_token, uint n) :
 PSViewToken(array_token),
 array_     ()
{
  array_ = new PSViewArray(*array_token.array_, n);
}

// TODO: transfer ownership
PSViewArrayToken::
PSViewArrayToken(CPSView *psview, PSViewArray *array) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_ARRAY,
             PSVIEW_TOKEN_COMPOSITE, PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 array_(array)
{
}

PSViewArrayToken::
~PSViewArrayToken()
{
}

PSViewArrayToken *
PSViewArrayToken::
dup() const
{
  return new PSViewArrayToken(*this);
}

int
PSViewArrayToken::
compare(PSViewToken *token)
{
  if (token->isType(type_))
    return array_->compare(dynamic_cast<PSViewArrayToken *>(token)->array_);
  else
    return type_ - token->getType();
}

PSVinteger
PSViewArrayToken::
getNumValues()
{
  return array_->getNumValues();
}

PSViewToken *
PSViewArrayToken::
getValue(uint i)
{
  return array_->getValue(i);
}

void
PSViewArrayToken::
setValue(uint i, PSViewToken *value)
{
  if (getGlobal() && value->getLocal()) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);
    return;
  }

  array_->setValue(i, value);
}

void
PSViewArrayToken::
setSubValues(PSVinteger pos, PSViewToken **sub_tokens,
             PSVinteger num_sub_tokens)
{
  if (num_sub_tokens <= 0)
    CTHROW("Invalid Number of Sub Values");

  if (getGlobal()) {
    for (PSVinteger i = 0; i < num_sub_tokens; ++i) {
      if (sub_tokens[i]->getLocal()) {
        psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);
        return;
      }
    }
  }

  array_->setSubValues(pos, sub_tokens, num_sub_tokens);
}

PSViewArrayToken *
PSViewArrayToken::
split(PSVinteger n)
{
  PSViewArray *array = array_->split(n);

  PSViewArrayToken *token = new PSViewArrayToken(psview_, array);

  return token;
}

void
PSViewArrayToken::
setBounds(PSVinteger pos, PSVinteger len)
{
  array_->setBounds(pos, len);
}

void
PSViewArrayToken::
executeToken()
{
  PSVinteger num_tokens = array_->getNumValues();

  if (num_tokens > 0) {
    psview_->getExecutionStack()->push(this);

    for (PSVinteger i = 1; i <= num_tokens; i++) {
      if (i == num_tokens)
        psview_->getExecutionStack()->pop();

      PSViewToken *sub_token = array_->getValue(i);

      if (sub_token->isProcedure())
        psview_->getOperandStack()->push(sub_token);
      else
        sub_token->execute();
    }
  }
}

const PSViewName &
PSViewArrayToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewArrayToken::
print()
{
  if (! isExecutable())
    CStrUtil::printf("[");
  else
    CStrUtil::printf("{");

  PSVinteger num_values = array_->getNumValues();

  for (PSVinteger i = 1; i <= num_values; ++i) {
    if (i > 1) CStrUtil::printf(" ");

    PSViewToken *token = array_->getValue(i);

    token->print();
  }

  if (! isExecutable())
    CStrUtil::printf("]");
  else
    CStrUtil::printf("}");
}

string
PSViewArrayToken::
toString()
{
  return "--nostringval--";
}
