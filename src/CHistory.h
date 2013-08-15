#ifndef CHISTORY_H
#define CHISTORY_H

#include <string>
#include <vector>
#include <deque>

class CHistoryCommand;

class CHistory {
 public:
  typedef std::deque<CHistoryCommand *> CommandList;

 private:
  int         command_num_;
  int         command_pos_;
  CommandList commands_;

 public:
  CHistory();
 ~CHistory();

  int getCommandNum() const { return command_num_; }

  void addCommand(const std::string &command);

  bool getCommand(int num, std::string &command);

  bool getCommandArg(int num, int arg_num, std::string &command,
                     std::string &arg);

  bool findCommandStart(const std::string &str, std::string &command,
                        int &command_num);
  bool findCommandIn(const std::string &str, std::string &command,
                     int &command_num);

  bool findCommandArg(const std::string &str, int &command_num, int &arg_num);

  bool save(const std::string &fileName);
  bool restore(const std::string &fileName);

  bool addFile(const std::string &fileName);

  void display(int num=-1, bool show_numbers=false,
               bool show_time=false, bool reverse=false);

  void resize(int size) const;

  int getFirstCommandNum();
  int getLastCommandNum();

  bool getPrevCommand(std::string &command);
  bool getNextCommand(std::string &command);

  void getCommands(std::vector<std::string> &commands) const;

  CommandList::const_iterator beginCommand() const;
  CommandList::const_iterator endCommand() const;

 private:
  bool getCommandP(int num, CHistoryCommand **pcommand);

  bool findCommandArg(const CHistoryCommand &cmd, const std::string &str,
                      int &command_num, int &arg_num) const;
};

class CHistoryCommand {
 public:
  typedef std::vector<std::string> WordList;

 private:
  std::string command_;
  int         number_;
  bool        words_set_;
  WordList    words_;
  time_t      time_stamp_;

 public:
  CHistoryCommand(const std::string &command, int number);
 ~CHistoryCommand();

  std::string getCommand() const;

  int getNumber () const;

  std::string getTimeString() const;

  const WordList &getWords() const;
};

#endif
