#include <CPSViewI.h>

PSViewIntegerToken::
PSViewIntegerToken(CPSView *psview, PSVinteger value) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_INTEGER,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 value_(value)
{
}

PSViewIntegerToken::
PSViewIntegerToken(const PSViewIntegerToken &integer_token) :
 PSViewToken(integer_token), value_(integer_token.value_)
{
}

PSViewIntegerToken::
~PSViewIntegerToken()
{
}

PSViewIntegerToken *
PSViewIntegerToken::
dup() const
{
  return new PSViewIntegerToken(*this);
}

int
PSViewIntegerToken::
compare(PSViewToken *token)
{
  if     (token->isType(type_)) {
    auto *integer_token = dynamic_cast<PSViewIntegerToken *>(token);

    return int(value_ - integer_token->value_);
  }
  else if (token->isType(PSVIEW_TOKEN_TYPE_REAL)) {
    PSViewRealToken real_token1(psview_, double(value_));

    return real_token1.compare(token);
  }
  else
    return type_ - token->getType();
}

const PSViewName &
PSViewIntegerToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewIntegerToken::
executeToken()
{
  psview_->getOperandStack()->push(this);
}

void
PSViewIntegerToken::
print()
{
  CStrUtil::printf("%ld", value_);
}

std::string
PSViewIntegerToken::
toString()
{
  return CStrUtil::toString(value_);
}
