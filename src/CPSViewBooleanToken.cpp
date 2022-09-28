#include <CPSViewI.h>

PSViewBooleanToken::
PSViewBooleanToken(CPSView *psview, PSVboolean value) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_BOOLEAN,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 value_(value)
{
}

PSViewBooleanToken::
PSViewBooleanToken(const PSViewBooleanToken &boolean_token) :
 PSViewToken(boolean_token), value_(boolean_token.value_)
{
}

PSViewBooleanToken::
~PSViewBooleanToken()
{
}

PSViewBooleanToken *
PSViewBooleanToken::
dup() const
{
  return new PSViewBooleanToken(*this);
}

int
PSViewBooleanToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    auto *boolean_token = dynamic_cast<PSViewBooleanToken *>(token);

    if      (value_ == boolean_token->value_)
      return 0;
    else if (value_)
      return 1;
    else
      return -1;
  }
  else
    return type_ - token->getType();
}

void
PSViewBooleanToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for Boolean Missing\n");
}

const PSViewName &
PSViewBooleanToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewBooleanToken::
print()
{
  CStrUtil::printf("%s", value_ ? "true" : "false");
}

std::string
PSViewBooleanToken::
toString()
{
  return (value_ ? "true" : "false");
}
