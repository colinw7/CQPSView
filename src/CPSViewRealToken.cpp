#include <CPSViewI.h>

PSViewRealToken::
PSViewRealToken(CPSView *psview, PSVreal value) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_REAL,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 value_(value)
{
}

PSViewRealToken::
PSViewRealToken(const PSViewRealToken &real_token) :
 PSViewToken(real_token), value_(real_token.value_)
{
}

PSViewRealToken::
~PSViewRealToken()
{
}

PSViewRealToken *
PSViewRealToken::
dup() const
{
  return new PSViewRealToken(*this);
}

int
PSViewRealToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    auto *real_token = dynamic_cast<PSViewRealToken *>(token);

    if      (value_ == real_token->value_)
      return 0;
    else if (value_ > real_token->value_)
      return 1;
    else
      return -1;
  }
  else if (token->isType(PSVIEW_TOKEN_TYPE_INTEGER)) {
    auto *integer_token = dynamic_cast<PSViewIntegerToken *>(token);

    PSViewRealToken real_token1(psview_, double(integer_token->getValue()));

    return compare(&real_token1);
  }
  else
    return type_ - token->getType();
}

void
PSViewRealToken::
executeToken()
{
  psview_->getOperandStack()->push(this);
}

const PSViewName &
PSViewRealToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewRealToken::
print()
{
  static char buffer[128];

  sprintf(buffer, "%g", value_);

  char *p = strchr(buffer, '.');

  if (p != NULL)
    CStrUtil::printf("%s", buffer);
  else
    CStrUtil::printf("%s.0", buffer);
}

std::string
PSViewRealToken::
toString()
{
  return CStrUtil::toString(value_);
}
