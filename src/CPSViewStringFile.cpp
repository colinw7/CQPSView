#include "CPSViewI.h"

PSViewStringFile::
PSViewStringFile(CPSView *psview, const string &str) :
 PSViewFile  (psview),
 buffer_     (),
 buffer_pos_ (0),
 buffer_size_()
{
  uint len = str.size();

  buffer_.resize(len + 1);

  memcpy(&buffer_[0], str.c_str(), len + 1);

  buffer_size_ = len;
}

PSViewStringFile::
~PSViewStringFile()
{
}

int
PSViewStringFile::
compare(PSViewStringFile *)
{
  return 1;
}

void
PSViewStringFile::
flush()
{
}

bool
PSViewStringFile::
isReadable() const
{
  return true;
}

bool
PSViewStringFile::
isWritable() const
{
  return false;
}

bool
PSViewStringFile::
isValid() const
{
  return (buffer_size_ > 0);
}

int
PSViewStringFile::
size()
{
  return buffer_size_;
}

int
PSViewStringFile::
bytesUsed()
{
  return buffer_pos_;
}

int
PSViewStringFile::
bytesAvailable()
{
  return (buffer_size_ - buffer_pos_);
}

bool
PSViewStringFile::
setPosition(uint pos)
{
  if (pos >= buffer_size_)
    return false;

  buffer_pos_ = pos;

  return true;
}

bool
PSViewStringFile::
getPosition(uint *pos)
{
  *pos = buffer_pos_;

  return true;
}

string
PSViewStringFile::
getFileName()
{
  return "--stringfile--";
}

int
PSViewStringFile::
readChar()
{
  if (buffer_pos_ >= buffer_size_)
    return EOF;

  int c = buffer_[buffer_pos_++];

  return c;
}

int
PSViewStringFile::
lookChar()
{
  if (buffer_pos_ >= buffer_size_)
    return EOF;

  int c = buffer_[buffer_pos_];

  return c;
}

void
PSViewStringFile::
unreadChars(const std::vector<int> &chars)
{
  uint len = chars.size();

  if (buffer_pos_ >= len)
    buffer_pos_ -= len;
  else
    assert(false);
}

bool
PSViewStringFile::
loadBuffer()
{
  return false;
}

bool
PSViewStringFile::
writeChar(int)
{
  return false;
}

void
PSViewStringFile::
reset()
{
  buffer_pos_  = 0;
  buffer_size_ = 0;
}

void
PSViewStringFile::
close()
{
  buffer_pos_  = 0;
  buffer_size_ = 0;
}
