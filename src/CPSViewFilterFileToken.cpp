#include <CPSViewI.h>

PSViewFilterFileToken::
PSViewFilterFileToken(PSViewFileToken *token, const PSViewName &name) :
 PSViewFileToken(token->getPSView()),
 filter_file_    ()
{
  PSViewFile *file = token->getFile();

  filter_file_ = new PSViewFilterFile(file, name);

  file_ = filter_file_;
}

PSViewFilterFileToken::
PSViewFilterFileToken(PSViewFileToken *token, const std::string &name) :
 PSViewFileToken(token->getPSView()),
 filter_file_    ()
{
  PSViewFile *file = token->getFile();

  filter_file_ = new PSViewFilterFile(file, name);

  file_ = filter_file_;
}

PSViewFilterFileToken::
PSViewFilterFileToken(const PSViewFilterFileToken &filter_file_token) :
 PSViewFileToken(filter_file_token),
 filter_file_    ()
{
  filter_file_ = new PSViewFilterFile(*filter_file_token.filter_file_);

  file_ = filter_file_;
}

PSViewFilterFileToken::
~PSViewFilterFileToken()
{
}

PSViewFilterFileToken *
PSViewFilterFileToken::
dup() const
{
  return new PSViewFilterFileToken(*this);
}

int
PSViewFilterFileToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    auto *filter_file_token = dynamic_cast<PSViewFilterFileToken *>(token);

    return filter_file_->compare(filter_file_token->filter_file_);
  }
  else
    return type_ - token->getType();
}

void
PSViewFilterFileToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for Filter File Missing\n");
}

void
PSViewFilterFileToken::
print()
{
  CStrUtil::printf("-file-");
}

std::string
PSViewFilterFileToken::
toString()
{
  return "--nostringval--";
}

void
PSViewFilterFileToken::
flush()
{
  filter_file_->flush();
}

PSViewToken *
PSViewFilterFileToken::
readToken()
{
  return filter_file_->readToken();
}

bool
PSViewFilterFileToken::
isReadable() const
{
  return filter_file_->isReadable();
}

bool
PSViewFilterFileToken::
isWritable() const
{
  return filter_file_->isWritable();
}

bool
PSViewFilterFileToken::
isValid() const
{
  return filter_file_->isValid();
}

int
PSViewFilterFileToken::
size()
{
  return filter_file_->size();
}

int
PSViewFilterFileToken::
bytesUsed()
{
  return filter_file_->bytesUsed();
}

uint
PSViewFilterFileToken::
bytesAvailable()
{
  return uint(filter_file_->bytesAvailable());
}

PSVboolean
PSViewFilterFileToken::
setPosition(uint pos)
{
  return filter_file_->setPosition(pos);
}

PSVboolean
PSViewFilterFileToken::
getPosition(uint *pos)
{
  return filter_file_->getPosition(pos);
}

std::string
PSViewFilterFileToken::
getFileName()
{
  return filter_file_->getFileName();
}

int
PSViewFilterFileToken::
readChar()
{
  return filter_file_->readChar();
}

int
PSViewFilterFileToken::
lookChar()
{
  return filter_file_->lookChar();
}

bool
PSViewFilterFileToken::
loadBuffer()
{
  return filter_file_->loadBuffer();
}

PSVboolean
PSViewFilterFileToken::
writeChar(PSVchar c)
{
  return filter_file_->writeChar(int(c));
}

void
PSViewFilterFileToken::
reset()
{
  filter_file_->reset();
}

void
PSViewFilterFileToken::
close()
{
  filter_file_->close();
}

const PSViewName &
PSViewFilterFileToken::
getName()
{
  return psview_->getNameMgr()->getName(filter_file_->getFileName());
}
