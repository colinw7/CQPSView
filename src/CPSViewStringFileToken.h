#ifndef CPSViewStringFileToken_H
#define CPSViewStringFileToken_H

class PSViewStringFileToken : public PSViewFileToken {
 public:
  PSViewStringFileToken(CPSView *psview, const std::string &str);
  PSViewStringFileToken(const PSViewStringFileToken &string_file_token);

  //----

  // virtual functions

  ~PSViewStringFileToken();

  PSViewStringFileToken *dup() const override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  void executeToken() override;

  int compare(PSViewToken *token) override;

  //----

  void         flush() override;
  PSViewToken *readToken() override;
  bool         isReadable() const;
  bool         isWritable() const;
  bool         isValid() const override;
  int          size();
  int          bytesUsed();
  uint         bytesAvailable() override;
  PSVboolean   setPosition(uint pos) override;
  PSVboolean   getPosition(uint *pos) override;
  std::string  getFileName() override;
  int          readChar() override;
  int          lookChar();
  bool         loadBuffer();
  PSVboolean   writeChar(PSVchar c) override;
  void         reset() override;
  void         close() override;

  PSViewStringFile *getStringFile() { return string_file_; }

 private:
  CAutoPtr<PSViewStringFile> string_file_;
};

#endif
