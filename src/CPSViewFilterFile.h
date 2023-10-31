#ifndef CPSViewFilterFile_H
#define CPSViewFilterFile_H

typedef int (*PSViewFilterFileProc)(...);

class PSViewFilterFile : public PSViewFile {
 public:
  PSViewFilterFile(PSViewFile *file, const PSViewName &name);
  PSViewFilterFile(PSViewFile *file, const std::string &name);
  PSViewFilterFile(const PSViewFilterFile &filter_file);
 ~PSViewFilterFile();

  int compare(PSViewFilterFile *filter_file);

  void flush() override;

  bool isReadable() const override;
  bool isWritable() const override;

  bool isValid() const override;

  int size() override;

  int bytesUsed() override;

  int bytesAvailable() override;

  bool setPosition(uint pos) override;

  bool getPosition(uint *pos) override;

  std::string getFileName() override;

  int lookChar() override;
  int readChar() override;

  void unreadChars(const std::vector<int> &chars) override;

  bool loadBuffer() override;

  bool writeChar(int c) override;

  void reset() override;

  void close() override;

 private:
  void init();

  static int asciiHexEncode(PSViewFilterFile *file, int c, char *data);
  static int asciiHexDecode(PSViewFilterFile *file, char *data, bool consume);
  static int ascii85Encode(PSViewFilterFile *file, int c, char *data);
  static int eexecDecode(PSViewFilterFile *file, char *data, bool consume);

 private:
  PSViewFilterFile &operator=(const PSViewFilterFile &rhs);

 private:
  PSViewFile           *file_ { nullptr };
  std::string           name_;
  PSViewFilterFileProc  proc_;
  char                 *data_ { nullptr };
  PSViewFileModeType    mode_;
  std::vector<char>     buffer_;
  uint                  buffer_pos_ { 0 };
};

#endif
