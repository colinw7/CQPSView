#include <CPSViewI.h>

PSViewNameToken::
PSViewNameToken(CPSView *psview, const PSViewName &value) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_NAME,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_EXECUTABLE,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 value_(value)
{
}

PSViewNameToken::
PSViewNameToken(CPSView *psview, const string &str) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_NAME,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_EXECUTABLE,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 value_(psview->getNameMgr()->getName(str))
{
}

PSViewNameToken::
PSViewNameToken(PSViewStringToken *token) :
 PSViewToken(token->getPSView(), PSVIEW_TOKEN_TYPE_NAME,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_EXECUTABLE,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 value_(token->getPSView()->getNameMgr()->getName(token->toString()))
{
}

PSViewNameToken::
PSViewNameToken(const PSViewNameToken &name_token) :
 PSViewToken(name_token), value_(name_token.value_)
{
}

PSViewNameToken::
~PSViewNameToken()
{
}

PSViewNameToken *
PSViewNameToken::
dup() const
{
  return new PSViewNameToken(*this);
}

int
PSViewNameToken::
compare(PSViewToken *token)
{
  if      (token->isType(type_)) {
    PSViewNameToken *name_token = dynamic_cast<PSViewNameToken *>(token);

    return value_.compare(name_token->value_);
  }
  else if (token->isType(PSVIEW_TOKEN_TYPE_STRING)) {
    PSViewStringToken *string_token = dynamic_cast<PSViewStringToken *>(token);

    PSViewNameToken name_token1 = PSViewNameToken(psview_, string_token->toString());

    return name_token1.compare(this);
  }
  else
    return type_ - token->getType();
}

void
PSViewNameToken::
executeToken()
{
  PSViewToken *value = psview_->getDictionaryMgr()->lookup(value_);

  if (value == NULL) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

    return;
  }

  if (value->isExecutable())
    value->execute();
  else
    psview_->getOperandStack()->push(value);
}

const PSViewName &
PSViewNameToken::
getName()
{
  return value_;
}

void
PSViewNameToken::
print()
{
  if (! isExecutable())
    CStrUtil::printf("/");

  CStrUtil::printf("%s" , value_.getString().c_str());
}

string
PSViewNameToken::
toString()
{
  return value_.getString();
}

const string &
PSViewNameToken::
getString()
{
  return value_.getString();
}

int
PSViewNameToken::
getLength()
{
  return value_.getLength();
}
