enum PSViewFileModeType {
  PSVIEW_FILE_MODE_TYPE_NONE       = 0,
  PSVIEW_FILE_MODE_TYPE_READ       = (1<<1),
  PSVIEW_FILE_MODE_TYPE_WRITE      = (1<<2),
  PSVIEW_FILE_MODE_TYPE_READ_WRITE = PSVIEW_FILE_MODE_TYPE_READ |
                                     PSVIEW_FILE_MODE_TYPE_WRITE
};

class PSViewFile {
 private:
  static std::string ascii85_chars_;
  static std::string delim_chars_;

  CPSView *psview_;

 public:
  PSViewFile(CPSView *psview);
  PSViewFile(const PSViewFile &rhs);

  CPSView *getPSView() const { return psview_; }

  virtual ~PSViewFile();

  virtual void flush() = 0;

  virtual bool isReadable() const = 0;
  virtual bool isWritable() const = 0;

  virtual bool isValid() const = 0;

  virtual int size() = 0;

  virtual int bytesUsed() = 0;

  virtual int bytesAvailable() = 0;

  virtual bool setPosition(uint pos) = 0;

  virtual bool getPosition(uint *pos) = 0;

  virtual std::string getFileName() = 0;

  virtual int lookChar() = 0;
  virtual int readChar() = 0;

  virtual void unreadChars(const std::vector<int> &chars) = 0;

  virtual bool loadBuffer() = 0;

  virtual bool writeChar(int c) = 0;

  virtual void reset() = 0;

  virtual void close() = 0;

  PSViewToken *readToken();
  PSViewToken *readNumber();
  PSViewToken *readString();
  PSViewToken *readName();
  PSViewToken *readProcedure();

  void skipComment();
  void skipSpace();

  PSVboolean isBaseDigit(PSVinteger base);
  PSVboolean isDigit();
  PSVboolean isSeparator();
  PSVboolean isDelimiter();

  std::string charsToASCII85(uint chars);

  PSVboolean isASCII85Char(int c);

 private:
  PSViewFile &operator=(const PSViewFile &rhs);
};
