#include <CPSViewI.h>

PSViewNullToken::
PSViewNullToken(CPSView *psview) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_NULL,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE)
{
}

PSViewNullToken::
PSViewNullToken(const PSViewNullToken &null_token) :
 PSViewToken(null_token)
{
}

PSViewNullToken::
~PSViewNullToken()
{
}

PSViewNullToken *
PSViewNullToken::
dup() const
{
 return new PSViewNullToken(psview_);
}

int
PSViewNullToken::
compare(PSViewToken *token)
{
  return type_ - token->getType();
}

void
PSViewNullToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for NULL Missing\n");
}

const PSViewName &
PSViewNullToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewNullToken::
print()
{
  CStrUtil::printf("null");
}

std::string
PSViewNullToken::
toString()
{
  return "--nostringval--";
}
