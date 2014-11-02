#include <CPSViewI.h>

PSViewStringFileToken::
PSViewStringFileToken(CPSView *psview, const string &str) :
 PSViewFileToken(psview),
 string_file_   ()
{
  string_file_ = new PSViewStringFile(psview, str);

  file_ = string_file_;
}

PSViewStringFileToken::
PSViewStringFileToken(const PSViewStringFileToken &string_file_token) :
 PSViewFileToken(string_file_token),
 string_file_   ()
{
  string_file_ = new PSViewStringFile(*string_file_token.string_file_);

  file_ = string_file_;
}

PSViewStringFileToken::
~PSViewStringFileToken()
{
}

PSViewStringFileToken *
PSViewStringFileToken::
dup() const
{
  return new PSViewStringFileToken(*this);
}

int
PSViewStringFileToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    PSViewStringFileToken *string_file_token = dynamic_cast<PSViewStringFileToken *>(token);

    return string_file_->compare(string_file_token->string_file_);
  }
  else
    return type_ - token->getType();
}

void
PSViewStringFileToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for String File Missing\n");
}

const PSViewName &
PSViewStringFileToken::
getName()
{
  return psview_->getNameMgr()->getName(string_file_->getFileName());
}

void
PSViewStringFileToken::
print()
{
  CStrUtil::printf("-file-");
}

string
PSViewStringFileToken::
toString()
{
  return "--nostringval--";
}

void
PSViewStringFileToken::
flush()
{
  string_file_->flush();
}

PSViewToken *
PSViewStringFileToken::
readToken()
{
  return string_file_->readToken();
}

bool
PSViewStringFileToken::
isReadable() const
{
  return string_file_->isReadable();
}

bool
PSViewStringFileToken::
isWritable() const
{
  return string_file_->isWritable();
}

bool
PSViewStringFileToken::
isValid() const
{
  return string_file_->isValid();
}

int
PSViewStringFileToken::
size()
{
  return string_file_->size();
}

int
PSViewStringFileToken::
bytesUsed()
{
  return string_file_->bytesUsed();
}

uint
PSViewStringFileToken::
bytesAvailable()
{
  return string_file_->bytesAvailable();
}

PSVboolean
PSViewStringFileToken::
setPosition(uint pos)
{
  return string_file_->setPosition(pos);
}

PSVboolean
PSViewStringFileToken::
getPosition(uint *pos)
{
  return string_file_->getPosition(pos);
}

string
PSViewStringFileToken::
getFileName()
{
  return string_file_->getFileName();
}

int
PSViewStringFileToken::
readChar()
{
  return string_file_->readChar();
}

int
PSViewStringFileToken::
lookChar()
{
  return string_file_->lookChar();
}

bool
PSViewStringFileToken::
loadBuffer()
{
  return string_file_->loadBuffer();
}

PSVboolean
PSViewStringFileToken::
writeChar(PSVchar c)
{
  return string_file_->writeChar(c);
}

void
PSViewStringFileToken::
reset()
{
  string_file_->reset();
}

void
PSViewStringFileToken::
close()
{
  string_file_->close();
}
