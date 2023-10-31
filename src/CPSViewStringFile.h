#ifndef CPSViewStringFile_H
#define CPSViewStringFile_H

class PSViewStringFile : public PSViewFile {
 public:
  PSViewStringFile(CPSView *psview, const std::string &str);
 ~PSViewStringFile();

  int compare(PSViewStringFile *string_file);

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

  int readChar() override;
  int lookChar() override;

  void unreadChars(const std::vector<int> &chars) override;

  bool loadBuffer() override;

  bool writeChar(int c) override;

  void reset() override;

  void close() override;

 private:
  std::vector<char> buffer_;
  uint              buffer_pos_;
  uint              buffer_size_;
};

#endif
