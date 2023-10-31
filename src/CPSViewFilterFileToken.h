#ifndef CPSViewFilterFileToken_H
#define CPSViewFilterFileToken_H

class PSViewFilterFileToken : public PSViewFileToken {
 public:
  PSViewFilterFileToken(PSViewFileToken *token, const PSViewName &name);
  PSViewFilterFileToken(PSViewFileToken *token, const std::string &name);
  PSViewFilterFileToken(const PSViewFilterFileToken &filter_file_token);

  //----

  // virtual functions

  ~PSViewFilterFileToken();

  PSViewFilterFileToken *dup() const override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  void executeToken() override;

  int compare(PSViewToken *token) override;

  //----

  void          flush() override;
  PSViewToken  *readToken() override;
  bool          isReadable() const;
  bool          isWritable() const;
  bool          isValid() const override;
  int           size();
  int           bytesUsed();
  uint          bytesAvailable() override;
  PSVboolean    setPosition(uint pos) override;
  PSVboolean    getPosition(uint *pos) override;
  std::string   getFileName() override;
  int           readChar() override;
  int           lookChar();
  bool          loadBuffer();
  PSVboolean    writeChar(PSVchar c) override;
  void          reset() override;
  void          close() override;

  PSViewFilterFile *getFilterFile() { return filter_file_; }

 private:
  CAutoPtr<PSViewFilterFile> filter_file_;
};

#endif
