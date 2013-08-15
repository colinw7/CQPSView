#include "CPSViewI.h"

PSViewSaveToken::
PSViewSaveToken(CPSView *psview, void *data) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_SAVE,
             PSVIEW_TOKEN_NON_COMPOSITE, PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 save_      ()
{
  save_ = new PSViewSave(data);
}

PSViewSaveToken::
PSViewSaveToken(const PSViewSaveToken &save_token) :
 PSViewToken(save_token),
 save_      ()
{
  save_ = new PSViewSave(*save_token.save_);
}

PSViewSaveToken::
~PSViewSaveToken()
{
}

PSViewSaveToken *
PSViewSaveToken::
dup() const
{
  return new PSViewSaveToken(*this);
}

int
PSViewSaveToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    PSViewSaveToken *save_token = dynamic_cast<PSViewSaveToken *>(token);

    return save_->compare(save_token->save_);
  }
  else
    return type_ - token->getType();
}

void *
PSViewSaveToken::
getValue()
{
  return save_->getValue();
}

void
PSViewSaveToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for Save Missing\n");
}

const PSViewName &
PSViewSaveToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewSaveToken::
print()
{
  CStrUtil::printf("-save-");
}

string
PSViewSaveToken::
toString()
{
  return "--nostringval--";
}
