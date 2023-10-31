#ifndef CPSViewTextFile_H
#define CPSViewTextFile_H

class PSViewTextFile : public PSViewFile {
 public:
  PSViewTextFile(CPSView *psview, const std::string &name, const std::string &mode);
  PSViewTextFile(const PSViewTextFile &text_file);
 ~PSViewTextFile();

  int compare(PSViewTextFile *text_file);

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
