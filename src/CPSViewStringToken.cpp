#include <CPSViewI.h>

PSViewStringToken::
PSViewStringToken(CPSView *psview, PSVinteger max_length) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_STRING,
             PSVIEW_TOKEN_COMPOSITE, PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 string_    ()
{
  string_ = new PSViewString(max_length);
}

PSViewStringToken::
PSViewStringToken(CPSView *psview, const string &str) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_STRING,
             PSVIEW_TOKEN_COMPOSITE, PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 string_    ()
{
  string_ = new PSViewString(str);
}

PSViewStringToken::
PSViewStringToken(CPSView *psview, PSViewString *str) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_STRING,
             PSVIEW_TOKEN_COMPOSITE, PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 string_    (str)
{
}

PSViewStringToken::
PSViewStringToken(const PSViewStringToken &string_token) :
 PSViewToken(string_token),
 string_    ()
{
  string_ = new PSViewString(*string_token.string_);
}

PSViewStringToken::
PSViewStringToken(const PSViewStringToken &string_token, int pos) :
 PSViewToken(string_token),
 string_    ()
{
  string_ = new PSViewString(*string_token.string_, pos);
}

PSViewStringToken::
PSViewStringToken(const PSViewStringToken &string_token, int pos, int len) :
 PSViewToken(string_token),
 string_    ()
{
  string_ = new PSViewString(*string_token.string_, pos, len);
}

PSViewStringToken::
~PSViewStringToken()
{
}

PSViewStringToken *
PSViewStringToken::
dup() const
{
  return new PSViewStringToken(*this);
}

int
PSViewStringToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    PSViewStringToken *string_token = dynamic_cast<PSViewStringToken *>(token);

    return string_->compare(*string_token->string_);
  }
  else if (token->isType(PSVIEW_TOKEN_TYPE_NAME)) {
    PSViewNameToken name_token1 = PSViewNameToken(psview_, string_->getString());

    return name_token1.compare(token);
  }
  else
    return type_ - token->getType();
}

void
PSViewStringToken::
executeToken()
{
  string str = string_->getString();

  PSViewStringFileToken *token1 = new PSViewStringFileToken(psview_, str);

  PSViewToken *token2 = token1->readToken();

  while (token2) {
    if (token2->isProcedure())
      psview_->getOperandStack()->push(token2);
    else
      token2->execute();

    if (psview_->getErrorMgr()->getError())
      break;

    token2 = token1->readToken();
  }

  token1->close();
}

const PSViewName &
PSViewStringToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewStringToken::
print()
{
  CStrUtil::printf("(");

  int len = string_->getLength();

  for (int i = 1; i <= len; ++i) {
    int c = string_->getChar(i);

    if      (c == '(' || c == ')')
      CStrUtil::printf("\\%c", c);
    else if (isprint(c))
      CStrUtil::printf("%c", c);
    else
      CStrUtil::printf("\\%03o", c);
  }

  CStrUtil::printf(")");
}

string
PSViewStringToken::
toString()
{
  return string_->getString();
}

PSViewStringToken *
PSViewStringToken::
split(PSVinteger n)
{
  PSViewString *str = string_->split(n);

  PSViewStringToken *token = new PSViewStringToken(psview_, str);

  return token;
}

int
PSViewStringToken::
compare(PSViewToken *token) const
{
  if (token->isType(type_)) {
    PSViewStringToken *string_token = dynamic_cast<PSViewStringToken *>(token);

    return string_->compare(*string_token->string_);
  }
  else
    return type_ - token->getType();
}

int
PSViewStringToken::
compareN(const PSViewStringToken *token, PSVinteger n) const
{
  return string_->compareN(*token->string_, n);
}

PSVinteger
PSViewStringToken::
isSubString(PSViewStringToken *token)
{
  return string_->isSubString(token->string_);
}

PSViewStringToken *
PSViewStringToken::
subString(PSVinteger start, PSVinteger num_chars)
{
  PSViewStringToken *token = new PSViewStringToken(psview_, string_->subString(start, num_chars));

  return token;
}

void
PSViewStringToken::
setString(const string &str)
{
  string_->setString(str);
}

void
PSViewStringToken::
setChar(int pos, char c)
{
  string_->setChar(pos, c);
}

void
PSViewStringToken::
setChars(const string &str, int pos)
{
  string_->setChars(str, pos);
}

void
PSViewStringToken::
setBounds(PSVinteger pos, PSVinteger len)
{
  string_->setBounds(pos, len);
}

int
PSViewStringToken::
getLength() const
{
  return string_->getLength();
}

int
PSViewStringToken::
getChar(int i) const
{
  return string_->getChar(i);
}
