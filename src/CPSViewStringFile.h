#ifndef CPSViewStringFile_H
#define CPSViewStringFile_H

class PSViewStringFile : public PSViewFile {
 public:
  PSViewStringFile(CPSView *psview, const std::string &str);
 ~PSViewStringFile();

  int compare(PSViewStringFile *string_file);

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
  std::vector<char> buffer_;
  uint              buffer_pos_;
  uint              buffer_size_;
};

#endif
