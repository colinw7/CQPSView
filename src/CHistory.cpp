#include <CHistory.h>
#include <CStrUtil.h>
#include <CFile.h>
#include <CFuncs.h>
#include <COSTime.h>
#include <algorithm>

CHistory::
CHistory() :
 command_num_(0), command_pos_(0)
{
}

CHistory::
~CHistory()
{
  std::for_each(commands_.begin(), commands_.end(), CDeletePointer());
}

void
CHistory::
addCommand(const std::string &command)
{
  CHistoryCommand *pcommand = new CHistoryCommand(command, command_num_);

  command_num_++;

  commands_.push_back(pcommand);

  command_pos_ = command_num_;
}

bool
CHistory::
getCommand(int num, std::string &command)
{
  CHistoryCommand *pcommand;

  if (! getCommandP(num, &pcommand))
    return false;

  command = pcommand->getCommand();

  return true;
}

bool
CHistory::
getCommandP(int num, CHistoryCommand **pcommand)
{
  CommandList::iterator p1 = commands_.begin();
  CommandList::iterator p2 = commands_.end  ();

  for ( ; p1 != p2; ++p1) {
    if ((*p1)->getNumber() == num) {
      *pcommand = *p1;

      return true;
    }
  }

  return false;
}

bool
CHistory::
getCommandArg(int num, int arg_num, std::string &command, std::string &arg)
{
  CHistoryCommand *pcommand;

  if (! getCommandP(num, &pcommand))
    return false;

  command = pcommand->getCommand();

  const CHistoryCommand::WordList words = pcommand->getWords();

  if (arg_num < 0 || arg_num >= (int) words.size())
    return false;

  arg = words[arg_num];

  return true;
}

bool
CHistory::
findCommandStart(const std::string &str, std::string &command, int &command_num)
{
  CommandList::reverse_iterator p1 = commands_.rbegin();
  CommandList::reverse_iterator p2 = commands_.rend  ();

  for ( ; p1 != p2; ++p1) {
    command = (*p1)->getCommand();

    std::string::size_type pos = command.find(str);

    if (pos == 0) {
      command_num = (*p1)->getNumber();

      return true;
    }
  }

  return false;
}

bool
CHistory::
findCommandIn(const std::string &str, std::string &command, int &command_num)
{
  CommandList::reverse_iterator p1 = commands_.rbegin();
  CommandList::reverse_iterator p2 = commands_.rend  ();

  for ( ; p1 != p2; ++p1) {
    command = (*p1)->getCommand();

    std::string::size_type pos = command.find(str);

    if (pos != std::string::npos) {
      command_num = (*p1)->getNumber();

      return true;
    }
  }

  return false;
}

bool
CHistory::
findCommandArg(const std::string &str, int &command_num, int &arg_num)
{
  CommandList::reverse_iterator p1 = commands_.rbegin();
  CommandList::reverse_iterator p2 = commands_.rend  ();

  for ( ; p1 != p2; ++p1) {
    if (findCommandArg(**p1, str, command_num, arg_num))
      return true;
  }

  return false;
}

bool
CHistory::
findCommandArg(const CHistoryCommand &cmd, const std::string &str,
               int &command_num, int &arg_num) const
{
  const CHistoryCommand::WordList words = cmd.getWords();

  uint num_args = words.size();

  for (uint i = 0; i < num_args; i++) {
    std::string::size_type pos = words[i].find(str);

    if (pos != std::string::npos) {
      command_num = cmd.getNumber();
      arg_num     = i;

      return true;
    }
  }

  return false;
}

bool
CHistory::
save(const std::string &fileName)
{
  CFile file(fileName);

  CommandList::iterator p1 = commands_.begin();
  CommandList::iterator p2 = commands_.end  ();

  for ( ; p1 != p2; ++p1) {
    file.write((*p1)->getCommand() + "\n");
  }

  return true;
}

bool
CHistory::
restore(const std::string &fileName)
{
  command_num_ = 0;

  commands_.clear();

  return addFile(fileName);
}

bool
CHistory::
addFile(const std::string &fileName)
{
  CFile file(fileName);

  if (! file.exists())
    return false;

  std::string line;

  while (file.readLine(line)) {
    if (line.size() == 0 || line[0] == '#')
      continue;

    addCommand(line);
  }

  return true;
}

void
CHistory::
display(int num, bool show_numbers, bool show_time, bool reverse)
{
  int i = 0;

  if (! reverse) {
    CommandList::iterator p1 = commands_.begin();
    CommandList::iterator p2 = commands_.end  ();

    for ( ; p1 != p2; p1++, i++) {
      if (show_numbers)
        CStrUtil::printf("%6d ", (*p1)->getNumber());

      if (show_time)
        std::cout << (*p1)->getTimeString() << "  ";

      std::cout << (*p1)->getCommand() << std::endl;

      if (num > 0 && i == num - 1)
        break;
    }
  }
  else {
    CommandList::reverse_iterator p1 = commands_.rbegin();
    CommandList::reverse_iterator p2 = commands_.rend  ();

    for ( ; p1 != p2; p1++, i++) {
      if (show_numbers)
        CStrUtil::printf("%6d ", (*p1)->getNumber());

      if (show_time)
        std::cout << (*p1)->getTimeString() << "  ";

      std::cout << (*p1)->getCommand() << std::endl;

      if (num > 0 && i == num - 1)
        break;
    }
  }
}

void
CHistory::
resize(int size) const
{
  if (size < 0)
    size = 1;

  CHistory *th = const_cast<CHistory *>(this);

  while (th->commands_.size() > (unsigned int) size) {
    CHistoryCommand *command = *th->commands_.begin();

    delete command;

    th->commands_.pop_front();
  }
}

int
CHistory::
getFirstCommandNum()
{
  CommandList::iterator p1 = commands_.begin();
  CommandList::iterator p2 = commands_.end  ();

  if (p1 != p2)
    return (*p1)->getNumber();

  return 0;
}

int
CHistory::
getLastCommandNum()
{
  CommandList::reverse_iterator p1 = commands_.rbegin();
  CommandList::reverse_iterator p2 = commands_.rend  ();

  if (p1 != p2)
    return (*p1)->getNumber();

  return 0;
}

bool
CHistory::
getPrevCommand(std::string &command)
{
  if (command_pos_ <= 0)
    return false;

  command = commands_[--command_pos_]->getCommand();

  if (command_pos_ <= 0)
    command_pos_ = 1;

  return true;
}

bool
CHistory::
getNextCommand(std::string &command)
{
  if (command_pos_ >= (int) commands_.size())
    return false;

  command = commands_[command_pos_++]->getCommand();

  if (command_pos_ > (int) commands_.size())
    command_pos_ = (int) commands_.size();

  return true;
}

void
CHistory::
getCommands(std::vector<std::string> &commands) const
{
  CommandList::const_iterator p1 = commands_.begin();
  CommandList::const_iterator p2 = commands_.end  ();

  for ( ; p1 != p2; p1++)
    commands.push_back((*p1)->getCommand());
}

CHistory::CommandList::const_iterator
CHistory::
beginCommand() const
{
  return commands_.begin();
}

CHistory::CommandList::const_iterator
CHistory::
endCommand() const
{
  return commands_.end();
}

//----------------------

CHistoryCommand::
CHistoryCommand(const std::string &command, int number) :
 command_(command), number_(number), words_set_(false)
{
  time_stamp_ = time(NULL);
}

CHistoryCommand::
~CHistoryCommand()
{
}

std::string
CHistoryCommand::
getCommand() const
{
  return command_;
}

int
CHistoryCommand::
getNumber() const
{
  return number_;
}

std::string
CHistoryCommand::
getTimeString() const
{
  return COSTime::getTimeString(time_stamp_, "%H:%M");
}

const CHistoryCommand::WordList &
CHistoryCommand::
getWords() const
{
  if (! words_set_) {
    CHistoryCommand *th = const_cast<CHistoryCommand *>(this);

    CStrUtil::addWords(command_, th->words_);

    th->words_set_ = true;
  }

  return words_;
}
