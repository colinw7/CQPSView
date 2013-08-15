#include "CPSViewI.h"

PSViewFileToken::
PSViewFileToken(CPSView *psview) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_FILE,
             PSVIEW_TOKEN_COMPOSITE, PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 file_      ()
{
}

PSViewFileToken::
PSViewFileToken(const PSViewFileToken &file_token) :
 PSViewToken(file_token),
 file_      ()
{
}

PSViewFileToken::
~PSViewFileToken()
{
}

int
PSViewFileToken::
compare(PSViewToken *token)
{
  if (token->isType(type_))
    return 1;
  else
    return type_ - token->getType();
}

PSVboolean
PSViewFileToken::
initPostScript()
{
  if (! file_->isReadable())
    return false;

  int c = file_->readChar();

  if (c != '%')
    return false;

  c = file_->readChar();

  if (c != '!')
    return false;

  file_->skipComment();

  return true;
}

PSViewToken *
PSViewFileToken::
readString()
{
  return file_->readString();
}
