#ifndef CPSViewTextFile_H
#define CPSViewTextFile_H

class PSViewTextFile : public PSViewFile {
 public:
  PSViewTextFile(CPSView *psview, const std::string &name, const std::string &mode);
  PSViewTextFile(const PSViewTextFile &text_file);
 ~PSViewTextFile();

  int compare(PSViewTextFile *text_file);

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

  int readChar();
  int lookChar();

  void unreadChars(const std::vector<int> &chars);

  bool loadBuffer();

  bool writeChar(int c);

  void reset();

  void close();

 private:
  void init();

 private:
  PSViewTextFile &operator=(const PSViewTextFile &rhs);

 private:
  std::string         name_;
  std::string         mode_;
  PSViewFileModeType  mode_type_;
  FILE               *fp_;
  int                 size_;
  uint                pos_;
  std::vector<char>   buffer_;
  uint                buffer_pos_;
  uint                buffer_num_;
  uint                buffer_size_;
};

#endif
