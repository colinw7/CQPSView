#include "CPSViewI.h"

static char ps_id_chars[] = "%! \n";

class PSViewExecData {
  friend class PSViewFilterFile;

 private:
  ushort r_;
  int    initialised_;
  uint   pos_start_;
  uint   pos_end_;

 public:
  PSViewExecData();
 ~PSViewExecData();

  static uchar decrypt(uint cipher, ushort *r);
};

PSViewFilterFile::
PSViewFilterFile(PSViewFile *file, const PSViewName &name) :
 PSViewFile (file->getPSView()),
 file_      (file),
 name_      (name.getString()),
 proc_      (),
 data_      (NULL),
 mode_      (PSVIEW_FILE_MODE_TYPE_NONE),
 buffer_    (),
 buffer_pos_(0)
{
  init();
}

PSViewFilterFile::
PSViewFilterFile(PSViewFile *file, const string &name) :
 PSViewFile (file->getPSView()),
 file_      (file),
 name_      (name),
 proc_      (),
 data_      (NULL),
 mode_      (PSVIEW_FILE_MODE_TYPE_NONE),
 buffer_    (),
 buffer_pos_(0)
{
  init();
}

PSViewFilterFile::
PSViewFilterFile(const PSViewFilterFile &filter_file) :
 PSViewFile (filter_file.getPSView()),
 file_      (filter_file.file_),
 name_      (filter_file.name_),
 proc_      (),
 data_      (NULL),
 mode_      (PSVIEW_FILE_MODE_TYPE_NONE),
 buffer_    (),
 buffer_pos_(0)
{
  init();
}

PSViewFilterFile::
~PSViewFilterFile()
{
}

void
PSViewFilterFile::
init()
{
  int size = 0;

  if      (name_ == "ASCIIHexEncode") {
    proc_ = (PSViewFilterFileProc) asciiHexEncode;
    data_ = NULL;
    mode_ = PSVIEW_FILE_MODE_TYPE_WRITE;
    size  = 0;
  }
  else if (name_ == "ASCIIHexDecode") {
    proc_ = (PSViewFilterFileProc) asciiHexDecode;
    data_ = NULL;
    mode_ = PSVIEW_FILE_MODE_TYPE_READ;
    size  = 0;
  }
  else if (name_ == "ASCII85Encode") {
    proc_ = (PSViewFilterFileProc) ascii85Encode;
    data_ = NULL;
    mode_ = PSVIEW_FILE_MODE_TYPE_WRITE;
    size  = 4;
  }
  else if (name_ == "EExecDecode") {
    PSViewExecData *eexec_data = new PSViewExecData;

    proc_ = (PSViewFilterFileProc) eexecDecode;
    data_ = (char *) eexec_data;
    mode_ = PSVIEW_FILE_MODE_TYPE_READ;
    size  = 256;
  }
  else
    CTHROW("Invalid File Type" + name_);

  buffer_.resize(size + 1);

  buffer_pos_ = 0;
}

int
PSViewFilterFile::
compare(PSViewFilterFile *)
{
  return 1;
}

void
PSViewFilterFile::
flush()
{
  if (! isWritable())
    return;

  (*proc_)(this, EOF, data_);
}

bool
PSViewFilterFile::
isReadable() const
{
  return file_->isReadable();
}

bool
PSViewFilterFile::
isWritable() const
{
  if (! (mode_ & PSVIEW_FILE_MODE_TYPE_WRITE))
    return false;

  return file_->isWritable();
}

bool
PSViewFilterFile::
isValid() const
{
  return (file_ != NULL);
}

int
PSViewFilterFile::
size()
{
  if (file_ == NULL)
    return -1;

  return file_->size();
}

int
PSViewFilterFile::
bytesUsed()
{
  if (file_ == NULL)
    return -1;

  return file_->bytesUsed();
}

int
PSViewFilterFile::
bytesAvailable()
{
  if (file_ == NULL)
    return -1;

  return file_->bytesAvailable();
}

bool
PSViewFilterFile::
setPosition(uint pos)
{
  if (file_ == NULL)
    return false;

  uint pos1;

  if (! file_->getPosition(&pos1))
    return false;

  if (pos == pos1)
    return true;

  if (! file_->setPosition(pos))
    return false;

  buffer_pos_ = 0;

  return true;
}

bool
PSViewFilterFile::
getPosition(uint *pos)
{
  if (file_ == NULL)
    return false;

  return file_->getPosition(pos);
}

string
PSViewFilterFile::
getFileName()
{
  return "--filterfile--";
}

int
PSViewFilterFile::
readChar()
{
  if (! (mode_ & PSVIEW_FILE_MODE_TYPE_READ))
    return EOF;

  if (file_ == NULL)
    return EOF;

  int c = (*proc_)(this, data_, true);

  return c;
}

int
PSViewFilterFile::
lookChar()
{
  if (! (mode_ & PSVIEW_FILE_MODE_TYPE_READ))
    return EOF;

  if (file_ == NULL)
    return EOF;

  int c = (*proc_)(this, data_, false);

  return c;
}

void
PSViewFilterFile::
unreadChars(const vector<int> &chars)
{
  file_->unreadChars(chars);
}

bool
PSViewFilterFile::
loadBuffer()
{
  return false;
}

bool
PSViewFilterFile::
writeChar(int c)
{
  if (! (mode_ & PSVIEW_FILE_MODE_TYPE_WRITE))
    return false;

  (*proc_)(this, c, data_);

  return true;
}

void
PSViewFilterFile::
reset()
{
  buffer_pos_ = 0;
}

void
PSViewFilterFile::
close()
{
  if (! (mode_ & PSVIEW_FILE_MODE_TYPE_WRITE))
    return;

  (*proc_)(this, EOF, data_);

  file_->close();

  file_ = NULL;

  buffer_pos_ = 0;
}

void
PSViewFilterFile::
asciiHexEncode(PSViewFilterFile *file, int c, char *)
{
  if (c == EOF)
    return;

  string str = CStrUtil::toHexString(c, 2);

  file->file_->writeChar(str[0]);
  file->file_->writeChar(str[1]);
}

int
PSViewFilterFile::
asciiHexDecode(PSViewFilterFile *file, char *, bool consume)
{
  vector<int> chars;

  // skip space
  int c = file->file_->lookChar();

  while (c != EOF && isspace(c)) {
    c = file->file_->readChar();

    chars.push_back(c);

    c = file->file_->lookChar();
  }

  // read file hex char
  int c1 = file->file_->readChar();

  if (c1 == EOF) {
    if (! consume) file->file_->unreadChars(chars);
    return EOF;
  }

  chars.push_back(c1);

  // skip space
  c = file->file_->lookChar();

  while (c != EOF && isspace(c)) {
    c = file->file_->readChar();

    chars.push_back(c);

    c = file->file_->lookChar();
  }

  int c2 = file->file_->readChar();

  if (c2 == EOF) {
    if (! consume) file->file_->unreadChars(chars);
    return EOF;
  }

  chars.push_back(c2);

  uint v1, v2;

  if (! CStrUtil::decodeHexChar(c1, &v1) || ! CStrUtil::decodeHexChar(c2, &v2)) {
    if (! consume) file->file_->unreadChars(chars);
    return EOF;
  }

  int cc = ((v1 & 0xF) << 4) | (v2 & 0xF);

  if (! consume) file->file_->unreadChars(chars);

  return cc;
}

void
PSViewFilterFile::
ascii85Encode(PSViewFilterFile *file, int c, char *)
{
  if (c == EOF) {
    if (file->buffer_pos_ == 0)
      return;

    int pos = file->buffer_pos_;

    while (file->buffer_pos_ <= 3)
      file->buffer_[file->buffer_pos_++] = '\0';

    int chars = (file->buffer_[0] << 24) | (file->buffer_[1] << 16) |
                (file->buffer_[2] <<  8) |  file->buffer_[3];

    string str = file->charsToASCII85(chars);

    if (str == "z")
      str = "!!!!!";

    for (int i = 0; i < pos + 1; i++)
      file->file_->writeChar(str[i]);

    file->file_->writeChar('~');
    file->file_->writeChar('>');

    file->buffer_pos_ = 0;

    return;
  }

  if (file->buffer_pos_ <= 3)
    file->buffer_[file->buffer_pos_++] = c;

  if (file->buffer_pos_ == 4) {
    int chars = (file->buffer_[0] << 24) | (file->buffer_[1] << 16) |
                (file->buffer_[2] <<  8) |  file->buffer_[3];

    string str = file->charsToASCII85(chars);

    int len = str.size();

    for (int i = 0; i < len; i++)
      file->file_->writeChar(str[i]);

    file->buffer_pos_ = 0;
  }
}

int
PSViewFilterFile::
eexecDecode(PSViewFilterFile *file, char *data, bool consume)
{
  PSViewExecData *eexec_data = (PSViewExecData *) data;

  uint pos;

  file->getPosition(&pos);

  if (! eexec_data->initialised_) {
    eexec_data->initialised_ = true;

    eexec_data->pos_start_ = pos;
    eexec_data->pos_end_   = pos;

    int i = 0;

    while (true) {
      file->skipSpace();

      string str = "XX";

      str[0] = file->readChar();

      if (str[0] == EOF || ! isxdigit(str[0]))
        break;

      if (str[0] == '0') {
        int zero_count = 1;

        uint save_pos;

        file->getPosition(&save_pos);

        file->skipSpace();

        str[1] = file->lookChar();

        while (str[1] == '0') {
          zero_count++;

          file->readChar();

          file->skipSpace();

          str[1] = file->lookChar();
        }

        if (zero_count == 512)
          break;

        file->setPosition(save_pos);
      }

      file->skipSpace();

      str[1] = file->readChar();

      if (str[1] == EOF || ! isxdigit(str[1]))
        break;

      uint c;

      sscanf(str.c_str(), "%x", &c);

      c = PSViewExecData::decrypt(c, &eexec_data->r_);

      if (i < 4)
        c = ps_id_chars[i];

      if (i >= (int) file->buffer_.size())
        file->buffer_.resize(2*file->buffer_.size() + 3);

      file->buffer_[i++] = c;
    }

    file->buffer_[i] = EOF;

    uint save_pos;

    file->getPosition(&save_pos);

    eexec_data->pos_end_ = save_pos;

    file->setPosition(pos);
  }

  if (pos < eexec_data->pos_start_ || pos > eexec_data->pos_end_)
    return EOF;

  int i = (pos - eexec_data->pos_start_)/2;

  int c = file->buffer_[i];

  if (consume) {
    file->readChar();
    file->readChar();
  }

  return c;
}

PSViewExecData::
PSViewExecData() :
 r_          (55665),
 initialised_(false),
 pos_start_  (0),
 pos_end_    (0)
{
}

uchar
PSViewExecData::
decrypt(uint cipher, ushort *r)
{
  static ushort c1 = 52845;
  static ushort c2 = 22719;

  uchar plain = (cipher ^ (*r >> 8));

  *r = (cipher + *r)*c1 + c2;

  return plain;
}
