#include "CPSViewI.h"

PSViewTextFileToken::
PSViewTextFileToken(CPSView *psview, const string &filename, const string &mode) :
 PSViewFileToken(psview),
 text_file_     ()
{
  text_file_ = new PSViewTextFile(psview, filename, mode);

  file_ = text_file_;
}

PSViewTextFileToken::
PSViewTextFileToken(const PSViewTextFileToken &text_file_token) :
 PSViewFileToken(text_file_token),
 text_file_     ()
{
  text_file_ = new PSViewTextFile(*text_file_token.text_file_);

  file_ = text_file_;
}

PSViewTextFileToken::
~PSViewTextFileToken()
{
}

PSViewTextFileToken *
PSViewTextFileToken::
dup() const
{
  return new PSViewTextFileToken(*this);
}

int
PSViewTextFileToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    PSViewTextFileToken *text_file_token = dynamic_cast<PSViewTextFileToken *>(token);

    return text_file_->compare(text_file_token->text_file_);
  }
  else
    return type_ - token->getType();
}

void
PSViewTextFileToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for Text File Missing\n");
}

const PSViewName &
PSViewTextFileToken::
getName()
{
  return psview_->getNameMgr()->getName(text_file_->getFileName());
}

void
PSViewTextFileToken::
print()
{
  CStrUtil::printf("-file-");
}

string
PSViewTextFileToken::
toString()
{
  return "--nostringval--";
}

void
PSViewTextFileToken::
flush()
{
  text_file_->flush();
}

PSViewToken *
PSViewTextFileToken::
readToken()
{
  if (! text_file_->isReadable())
    return false;

  return text_file_->readToken();
}

bool
PSViewTextFileToken::
isValid() const
{
  return text_file_->isValid();
}

int
PSViewTextFileToken::
readChar()
{
  if (! text_file_->isReadable()) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);

    return EOF;
  }

  int c = text_file_->readChar();

  if (c == EOF)
    close();

  return c;
}

PSVboolean
PSViewTextFileToken::
writeChar(PSVchar c)
{
  return text_file_->writeChar(c);
}

void
PSViewTextFileToken::
reset()
{
  text_file_->reset();
}

void
PSViewTextFileToken::
close()
{
  text_file_->close();
}

uint
PSViewTextFileToken::
bytesAvailable()
{
  return text_file_->bytesAvailable();
}

PSVboolean
PSViewTextFileToken::
getPosition(uint *pos)
{
  if (! text_file_->getPosition(pos)) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_IO_ERROR);

    return false;
  }

  return true;
}

PSVboolean
PSViewTextFileToken::
setPosition(uint pos)
{
  return text_file_->setPosition(pos);
}

string
PSViewTextFileToken::
getFileName()
{
  return text_file_->getFileName();
}
