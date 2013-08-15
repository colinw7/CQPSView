#ifndef CREADLINE_H
#define CREADLINE_H

#include <CHistory.h>

struct CReadLineHistoryEntry {
  int         line_num;
  std::string line;

  CReadLineHistoryEntry(int line_num, const std::string &line) :
   line_num(line_num), line(line) {
  }
};

class CReadLine {
 private:
  std::string  prompt_;
  std::string  name_;
  bool         eof_;
  CHistory    *history_;

  static CReadLine *current_;

 public:
  CReadLine();

  virtual ~CReadLine() { }

  void setAutoHistory(bool flag);

  void enableTimoutHook();
  void disableTimoutHook();

  std::string readLine();

  std::string getPrompt() const { return prompt_; }

  void setPrompt(const std::string &prompt);

  void setName(const std::string &name);

  bool eof() { return eof_; }

  virtual bool completeLine(const std::string &, std::string &) { return false; }

  virtual bool showComplete(const std::string &) { return false; }

  virtual bool getPrevCommand(std::string &line);
  virtual bool getNextCommand(std::string &line);

  virtual void timeout() { }

  std::string getBuffer() const;

  void setBuffer(const std::string &str);

  void addHistory(const std::string &line);

  void getHistoryEntries(std::vector<CReadLineHistoryEntry> &entries);

  void beep();

  void interrupt();

 private:
  static int rlCompleteLine(int count, int key);
  static int rlShowComplete(int count, int key);
  static int rlPrevCommand(int count, int key);
  static int rlNextCommand(int count, int key);
  static int rlEventHook(void);

 private:
  CReadLine(const CReadLine &rhs);
  CReadLine &operator=(const CReadLine &rhs);
};

#endif
