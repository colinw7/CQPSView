#include <CReadLine.h>
#include <readline/readline.h>
#include <readline/history.h>

using std::string;
using std::vector;

CReadLine *CReadLine::current_ = NULL;

CReadLine::
CReadLine() :
 prompt_("> "), name_("readline"), eof_(false), history_(NULL)
{
  rl_readline_name = name_.c_str();

  rl_catch_signals = 0;

  rl_bind_key('\t'  , (int (*)(int, int)) rlCompleteLine);
  rl_bind_key('\033', (int (*)(int, int)) rlCompleteLine);

  rl_set_key("\\C-d", (int (*)(int, int)) rlShowComplete, rl_get_keymap());

#ifdef OS_WIN
  rl_set_key("\033[0A", (int (*)(int, int)) rlPrevCommand , rl_get_keymap());
  rl_set_key("\033[0B", (int (*)(int, int)) rlNextCommand , rl_get_keymap());
  rl_set_key("\033[0C", rl_named_function("forward-char" ), rl_get_keymap());
  rl_set_key("\033[0D", rl_named_function("backward-char"), rl_get_keymap());
#endif

  rl_set_key("\033[A", (int (*)(int, int)) rlPrevCommand , rl_get_keymap());
  rl_set_key("\033[B", (int (*)(int, int)) rlNextCommand , rl_get_keymap());
  rl_set_key("\033[C", rl_named_function("forward-char" ), rl_get_keymap());
  rl_set_key("\033[D", rl_named_function("backward-char"), rl_get_keymap());

  rl_set_key("\033[OA", (int (*)(int, int)) rlPrevCommand , rl_get_keymap());
  rl_set_key("\033[OB", (int (*)(int, int)) rlNextCommand , rl_get_keymap());
  rl_set_key("\033[OC", rl_named_function("forward-char" ), rl_get_keymap());
  rl_set_key("\033[OD", rl_named_function("backward-char"), rl_get_keymap());
}

void
CReadLine::
setAutoHistory(bool flag)
{
  delete history_;

  if (flag)
    history_ = new CHistory();
  else
    history_ = NULL;
}

void
CReadLine::
enableTimoutHook()
{
  rl_event_hook = rlEventHook;
}

void
CReadLine::
disableTimoutHook()
{
  rl_event_hook = NULL;
}

string
CReadLine::
readLine()
{
  current_ = this;

  char *p = ::readline((char *) prompt_.c_str());

  if (p == NULL) {
    eof_ = true;

    return "";
  }

  if (history_ != NULL)
    addHistory(p);

  return p;
}

void
CReadLine::
setPrompt(const string &prompt)
{
  prompt_ = prompt;
}

void
CReadLine::
setName(const string &name)
{
  name_ = name;

  rl_readline_name = name_.c_str();
}

bool
CReadLine::
getPrevCommand(string &line)
{
  if (history_ != NULL)
    return history_->getPrevCommand(line);
  else
    return false;
}

bool
CReadLine::
getNextCommand(string &line)
{
  if (history_ != NULL)
    return history_->getNextCommand(line);
  else
    return false;
}

int
CReadLine::
rlCompleteLine(int, int)
{
  if (rl_point != rl_end)
    return 0;

  string line = current_->getBuffer();

  string line1;

  if (current_->completeLine(line, line1))
    current_->setBuffer(line + line1);

  return 0;
}

int
CReadLine::
rlShowComplete(int, int)
{
  if (rl_point != rl_end) {
    rl_end = rl_point;

    rl_line_buffer[rl_end] = '\0';
  }
  else {
    string line = current_->getBuffer();

    if (current_->showComplete(line))
      rl_forced_update_display();
  }

  return 0;
}

int
CReadLine::
rlPrevCommand(int, int)
{
  string line;

  if (current_->getPrevCommand(line))
    current_->setBuffer(line);

  return 0;
}

int
CReadLine::
rlNextCommand(int, int)
{
  string line;

  if (! current_->getNextCommand(line))
    line = "";

  current_->setBuffer(line);

  return 0;
}

int
CReadLine::
rlEventHook()
{
  current_->timeout();

  return 1;
}

string
CReadLine::
getBuffer() const
{
  return rl_line_buffer;
}

void
CReadLine::
setBuffer(const string &buffer)
{
  rl_extend_line_buffer(buffer.size() + 1);

  strcpy(rl_line_buffer, buffer.c_str());

  rl_end = buffer.size();

  rl_point = rl_end;
}

void
CReadLine::
addHistory(const string &line)
{
  if (history_ == NULL)
    history_ = new CHistory;

  history_->addCommand(line);
}

void
CReadLine::
getHistoryEntries(vector<CReadLineHistoryEntry> &entries)
{
  HIST_ENTRY **hist_entries = history_list();

  for (int i = 0; hist_entries != NULL && hist_entries[i] != NULL; i++) {
    CReadLineHistoryEntry entry(i + history_base, hist_entries[i]->line);

    entries.push_back(entry);
  }
}

void
CReadLine::
beep()
{
  rl_ding();
}

void
CReadLine::
interrupt()
{
  rl_crlf();

  setBuffer("");

  rl_forced_update_display();
}
