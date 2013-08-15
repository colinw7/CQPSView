#include "CPSViewI.h"

PSViewOperatorToken::
PSViewOperatorToken(CPSView *psview, const string &name) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_OPERATOR,
             PSVIEW_TOKEN_NON_COMPOSITE, PSVIEW_TOKEN_EXECUTABLE,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 operator_  ()
{
  operator_ = psview_->getOperatorMgr()->lookup(name);
}

PSViewOperatorToken::
PSViewOperatorToken(CPSView *psview, PSViewOperator *opr) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_OPERATOR,
             PSVIEW_TOKEN_NON_COMPOSITE, PSVIEW_TOKEN_EXECUTABLE,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 operator_  ()
{
  operator_ = opr;
}

PSViewOperatorToken::
PSViewOperatorToken(const PSViewOperatorToken &operator_token) :
 PSViewToken(operator_token),
 operator_  (operator_token.operator_)
{
}

PSViewOperatorToken::
~PSViewOperatorToken()
{
}

PSViewOperatorToken *
PSViewOperatorToken::
dup() const
{
  return new PSViewOperatorToken(*this);
}

int
PSViewOperatorToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    PSViewOperatorToken *operator_token = dynamic_cast<PSViewOperatorToken *>(token);

    return operator_->compare(operator_token->operator_);
  }
  else
    return type_ - token->getType();
}

void
PSViewOperatorToken::
executeToken()
{
  operator_->invoke();
}

const PSViewName &
PSViewOperatorToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewOperatorToken::
print()
{
  CStrUtil::printf("--%s--", operator_->getName().getString().c_str());
}

string
PSViewOperatorToken::
toString()
{
  return operator_->getName().getString();
}
