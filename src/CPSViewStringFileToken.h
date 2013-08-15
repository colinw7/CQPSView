class PSViewStringFileToken : public PSViewFileToken {
 private:
  CAutoPtr<PSViewStringFile> string_file_;

 public:
  PSViewStringFileToken(CPSView *psview, const std::string &str);
  PSViewStringFileToken(const PSViewStringFileToken &string_file_token);

  //----

  // virtual functions

  ~PSViewStringFileToken();

  PSViewStringFileToken *dup() const;

  const PSViewName &getName();

  void print();

  std::string toString();

  void executeToken();

  int compare(PSViewToken *token);

  //----

  void         flush();
  PSViewToken *readToken();
  bool         isReadable() const;
  bool         isWritable() const;
  bool         isValid() const;
  int          size();
  int          bytesUsed();
  uint         bytesAvailable();
  PSVboolean   setPosition(uint pos);
  PSVboolean   getPosition(uint *pos);
  std::string  getFileName();
  int          readChar();
  int          lookChar();
  bool         loadBuffer();
  PSVboolean   writeChar(PSVchar c);
  void         reset();
  void         close();

  PSViewStringFile *getStringFile() { return string_file_; }
};
