#include <CPSViewI.h>

PSViewMarkToken::
PSViewMarkToken(CPSView *psview) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_MARK,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE)
{
}

PSViewMarkToken::
~PSViewMarkToken()
{
}

PSViewMarkToken *
PSViewMarkToken::
dup() const
{
  return const_cast<PSViewMarkToken *>(this);
}

int
PSViewMarkToken::
compare(PSViewToken *token)
{
  return type_ - token->getType();
}

void
PSViewMarkToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for Mark Missing\n");
}

const PSViewName &
PSViewMarkToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewMarkToken::
print()
{
  CStrUtil::printf("-mark-");
}

std::string
PSViewMarkToken::
toString()
{
  return "--nostringval--";
}
