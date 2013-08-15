#ifndef PSVIEW_FILTER_FILE_TOKEN_H
#define PSVIEW_FILTER_FILE_TOKEN_H

class PSViewFilterFileToken : public PSViewFileToken {
 private:
  CAutoPtr<PSViewFilterFile> filter_file_;

 public:
  PSViewFilterFileToken(PSViewFileToken *token, const PSViewName &name);
  PSViewFilterFileToken(PSViewFileToken *token, const std::string &name);
  PSViewFilterFileToken(const PSViewFilterFileToken &filter_file_token);

  //----

  // virtual functions

  ~PSViewFilterFileToken();

  PSViewFilterFileToken *dup() const;

  const PSViewName &getName();

  void print();

  std::string toString();

  void executeToken();

  int compare(PSViewToken *token);

  //----

  void          flush();
  PSViewToken  *readToken();
  bool          isReadable() const;
  bool          isWritable() const;
  bool          isValid() const;
  int           size();
  int           bytesUsed();
  uint          bytesAvailable();
  PSVboolean    setPosition(uint pos);
  PSVboolean    getPosition(uint *pos);
  std::string   getFileName();
  int           readChar();
  int           lookChar();
  bool          loadBuffer();
  PSVboolean    writeChar(PSVchar c);
  void          reset();
  void          close();

  PSViewFilterFile *getFilterFile() { return filter_file_; }
};

#endif
