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

  void flush();

  bool isReadable() const;
  bool isWritable() const;

  bool isValid() const;

  int size();

  int bytesUsed();

  int bytesAvailable();

  bool setPosition(uint pos);

  bool getPosition(uint *pos);

  std::string getFileName();

  int lookChar();
  int readChar();

  void unreadChars(const std::vector<int> &chars);

  bool loadBuffer();

  bool writeChar(int c);

  void reset();

  void close();

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
