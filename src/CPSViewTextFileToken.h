#ifndef CPSViewTextFileToken_H
#define CPSViewTextFileToken_H

class PSViewTextFileToken : public PSViewFileToken {
 public:
  PSViewTextFileToken(CPSView *psview, const std::string &filename, const std::string &mode);
  PSViewTextFileToken(const PSViewTextFileToken &text_file_token);

  //----

  // virtual functions

  ~PSViewTextFileToken();

  PSViewTextFileToken *dup() const override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  void executeToken() override;

  int compare(PSViewToken *token) override;

  //----

  void         flush() override;
  PSViewToken *readToken() override;
  bool         isValid() const override;
  uint         bytesAvailable() override;
  PSVboolean   getPosition(uint *pos) override;
  PSVboolean   setPosition(uint pos) override;
  std::string  getFileName() override;
  int          readChar() override;
  PSVboolean   writeChar(PSVchar c) override;
  void         reset() override;
  void         close() override;

  PSViewTextFile *getTextFile() { return text_file_; }

 private:
  CAutoPtr<PSViewTextFile> text_file_;
};

#endif
