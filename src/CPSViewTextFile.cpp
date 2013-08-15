#include "CPSViewI.h"
#include <COSFile.h>

const int BUFFER_SIZE = 512;

PSViewTextFile::
PSViewTextFile(CPSView *psview, const string &name, const string &mode) :
 PSViewFile  (psview),
 name_       (name),
 mode_       (mode),
 mode_type_  (PSVIEW_FILE_MODE_TYPE_NONE),
 fp_         (NULL),
 size_       (0),
 pos_        (0),
 buffer_     (),
 buffer_pos_ (0),
 buffer_num_ (0),
 buffer_size_(0)
{
  init();
}

PSViewTextFile::
PSViewTextFile(const PSViewTextFile &text_file) :
 PSViewFile  (text_file),
 name_       (text_file.name_),
 mode_       (text_file.mode_),
 mode_type_  (PSVIEW_FILE_MODE_TYPE_NONE),
 fp_         (NULL),
 size_       (0),
 pos_        (0),
 buffer_     (),
 buffer_pos_ (0),
 buffer_num_ (0),
 buffer_size_(0)
{
  init();
}

PSViewTextFile::
~PSViewTextFile()
{
  if (fp_ != stdin && fp_ != stdout && fp_ != stderr)
    fclose(fp_);
}

void
PSViewTextFile::
init()
{
  if      (mode_ == "r")
    mode_type_ = PSVIEW_FILE_MODE_TYPE_READ;
  else if (mode_ == "r+")
    mode_type_ = PSVIEW_FILE_MODE_TYPE_READ_WRITE;
  else if (mode_ == "w")
    mode_type_ = PSVIEW_FILE_MODE_TYPE_WRITE;
  else if (mode_ == "w+")
    mode_type_ = PSVIEW_FILE_MODE_TYPE_READ_WRITE;
  else if (mode_ == "a")
    mode_type_ = PSVIEW_FILE_MODE_TYPE_WRITE;
  else if (mode_ == "a+")
    mode_type_ = PSVIEW_FILE_MODE_TYPE_READ_WRITE;
  else
    CTHROW("Invalid File Mode" + mode_);

  if      (name_ == "%stdin")
    fp_ = stdin;
  else if (name_ == "%stdout")
    fp_ = stdout;
  else if (name_ == "%stderr")
    fp_ = stderr;
  else if (name_ == "%statementedit")
    fp_ = stdin;
  else if (name_ == "%lineedit")
    fp_ = stdin;
  else
    fp_ = fopen(name_.c_str(), mode_.c_str());

  if (fp_ == NULL)
    CTHROW("Can't open file " + name_);

  size_ = -1;

  if (fp_ != stdin && fp_ != stdout && fp_ != stderr) {
    struct stat file_stat;

    int error = fstat(fileno(fp_), &file_stat);

    if (error != 0 || ! COSFile::stat_is_reg(&file_stat)) {
      fclose(fp_);

      CTHROW("Not a regular File");
    }

    size_ = file_stat.st_size;
  }

  pos_ = 0;

  if (mode_type_ & PSVIEW_FILE_MODE_TYPE_READ)
    buffer_size_ = BUFFER_SIZE;
  else
    buffer_size_ = 0;

  buffer_.resize(buffer_size_ + 1);

  buffer_[0] = '\0';

  buffer_num_ = 0;
  buffer_pos_ = 0;
}

int
PSViewTextFile::
compare(PSViewTextFile *)
{
  return 1;
}

void
PSViewTextFile::
flush()
{
  if (! isWritable())
    return;

  if (fp_ != NULL)
    fflush(fp_);
}

bool
PSViewTextFile::
isReadable() const
{
  return (mode_type_ & PSVIEW_FILE_MODE_TYPE_READ);
}

bool
PSViewTextFile::
isWritable() const
{
  return (mode_type_ & PSVIEW_FILE_MODE_TYPE_WRITE);
}

bool
PSViewTextFile::
isValid() const
{
  return (fp_ != NULL);
}

int
PSViewTextFile::
size()
{
  return size_;
}

int
PSViewTextFile::
bytesUsed()
{
  if (fp_ == stdin || fp_ == stdout || fp_ == stderr)
    return -1;

  return(pos_ - buffer_num_ + buffer_pos_);
}

int
PSViewTextFile::
bytesAvailable()
{
  if (fp_ == stdin || fp_ == stdout || fp_ == stderr)
    return -1;

  return (size_ - bytesUsed());
}

bool
PSViewTextFile::
setPosition(uint pos)
{
  if ((int) pos >= size_)
    return false;

  if (fp_ == stdin  || fp_ == stdout || fp_ == stderr || fp_ == NULL)
    return false;

  buffer_pos_ = pos - pos_ + buffer_pos_;

  if (buffer_pos_ < buffer_num_) {
    pos_ = pos;

    return true;
  }

  fseek(fp_, pos, SEEK_SET);

  uint pos1 = ftell(fp_);

  if (pos1 != pos)
    return false;

  pos_ = pos;

  buffer_[0] = '\0';

  buffer_pos_ = 0;
  buffer_num_ = 0;

  return true;
}

bool
PSViewTextFile::
getPosition(uint *pos)
{
  if (fp_ == stdin || fp_ == stdout || fp_ == stderr || fp_ == NULL)
    return false;

  *pos = pos_;

  return true;
}

string
PSViewTextFile::
getFileName()
{
  return name_;
}

int
PSViewTextFile::
readChar()
{
  if (! (mode_type_ & PSVIEW_FILE_MODE_TYPE_READ))
    return EOF;

  if (fp_ == NULL)
    return EOF;

  if (buffer_pos_ >= buffer_num_) {
    if (! loadBuffer())
      return EOF;
  }

  pos_++;

  int c = buffer_[buffer_pos_++];

  return c;
}

int
PSViewTextFile::
lookChar()
{
  if (! (mode_type_ & PSVIEW_FILE_MODE_TYPE_READ))
    return EOF;

  if (fp_ == NULL)
    return EOF;

  if (buffer_pos_ >= buffer_num_) {
    if (! loadBuffer())
      return EOF;
  }

  int c = buffer_[buffer_pos_];

  return c;
}

void
PSViewTextFile::
unreadChars(const std::vector<int> &chars)
{
  uint len = chars.size();

  if (buffer_pos_ >= len)
    buffer_pos_ -= len;
  else
    assert(false);
}

bool
PSViewTextFile::
loadBuffer()
{
  if (! (mode_type_ & PSVIEW_FILE_MODE_TYPE_READ))
    return false;

  if (fp_ == NULL)
    return false;

  buffer_pos_ = 0;
  buffer_num_  = 0;

  int c;

  while (buffer_num_ < buffer_size_ && (c = fgetc(fp_)) != EOF)
    buffer_[buffer_num_++] = c;
  buffer_[buffer_num_] = '\0';

  if (buffer_num_ == 0)
    return false;

  return true;
}

bool
PSViewTextFile::
writeChar(int c)
{
  if (! (mode_type_ & PSVIEW_FILE_MODE_TYPE_WRITE))
    return false;

  if (fp_ == NULL)
    return false;

  fputc(c, fp_);

  return true;
}

void
PSViewTextFile::
reset()
{
  buffer_[0] = '\0';

  buffer_pos_ = 0;
  buffer_num_ = 0;
}

void
PSViewTextFile::
close()
{
  if (mode_type_ & PSVIEW_FILE_MODE_TYPE_READ) {
    buffer_pos_  = 0;
    buffer_num_  = 0;
    buffer_size_ = 0;
  }

  if (mode_type_ & PSVIEW_FILE_MODE_TYPE_WRITE && fp_ != NULL)
    fflush(fp_);

  if (fp_ != NULL   && fp_ != stdin &&
      fp_ != stdout && fp_ != stderr)
    fclose(fp_);

  fp_ = NULL;

  size_ = 0;
  pos_  = 0;
}
