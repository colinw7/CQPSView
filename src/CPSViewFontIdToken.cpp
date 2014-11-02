#include <CPSViewI.h>

PSViewFontIdToken::
PSViewFontIdToken(CPSView *psview, PSVfont_id value) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_FONT_ID,
             PSVIEW_TOKEN_NON_COMPOSITE,
             PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 value_(value)
{
}

PSViewFontIdToken::
PSViewFontIdToken(const PSViewFontIdToken &font_id_token) :
 PSViewToken(font_id_token), value_(font_id_token.value_)
{
}

PSViewFontIdToken::
~PSViewFontIdToken()
{
}

PSViewFontIdToken *
PSViewFontIdToken::
dup() const
{
  return new PSViewFontIdToken(*this);
}

int
PSViewFontIdToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    PSViewFontIdToken *font_id_token = dynamic_cast<PSViewFontIdToken *>(token);

    return (value_ - font_id_token->value_);
  }
  else
    return type_ - token->getType();
}

void
PSViewFontIdToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for Font Id Missing\n");
}

const PSViewName &
PSViewFontIdToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewFontIdToken::
print()
{
  CStrUtil::printf("-%s-", getTypeName().c_str());
}

string
PSViewFontIdToken::
toString()
{
  return "--nostringval--";
}
