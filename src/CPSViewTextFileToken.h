class PSViewTextFileToken : public PSViewFileToken {
 private:
  CAutoPtr<PSViewTextFile> text_file_;

 public:
  PSViewTextFileToken(CPSView *psview, const std::string &filename, const std::string &mode);
  PSViewTextFileToken(const PSViewTextFileToken &text_file_token);

  //----

  // virtual functions

  ~PSViewTextFileToken();

  PSViewTextFileToken *dup() const;

  const PSViewName &getName();

  void print();

  std::string toString();

  void executeToken();

  int compare(PSViewToken *token);

  //----

  void         flush();
  PSViewToken *readToken();
  bool         isValid() const;
  uint         bytesAvailable();
  PSVboolean   getPosition(uint *pos);
  PSVboolean   setPosition(uint pos);
  std::string  getFileName();
  int          readChar();
  PSVboolean   writeChar(PSVchar c);
  void         reset();
  void         close();

  PSViewTextFile *getTextFile() { return text_file_; }
};
