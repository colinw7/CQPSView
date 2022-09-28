#include <CPSViewI.h>
#include <CReadLine.h>

CPSView::
CPSView() :
 start_time_        (0),
 debug_             (false),
 exit_flag_         (false),
 memory_mgr_        (),
 name_mgr_          (),
 resource_mgr_      (),
 dictionary_mgr_    (),
 operator_mgr_      (),
 operand_stack_     (),
 execution_stack_   (),
 graphics_stack_    (),
 graphics_          (NULL),
 gstate_mgr_        (),
 gstate_token_mgr_  (),
 token_mgr_         (),
 error_mgr_         (),
 mark_token_        (),
 current_file_      (NULL),
 current_file_stack_()
{
  if (getenv("PSVIEW_DEBUG") != NULL)
    debug_ = true;

  memory_mgr_     = new PSViewMemoryMgr(this);
  name_mgr_       = new PSViewNameMgr  (this);
  resource_mgr_   = new PSViewResourceMgr(this);
  dictionary_mgr_ = new PSViewDictionaryMgr(this);
  operator_mgr_   = new PSViewOperatorMgr(this);

  operand_stack_   = new PSViewOperandStack  (this);
  execution_stack_ = new PSViewExecutionStack(this);
  graphics_stack_  = new PSViewGraphicsStack (this);

  gstate_mgr_       = new PSViewGStateMgr(this);
  gstate_token_mgr_ = new PSViewGStateTokenMgr(this);

  token_mgr_ = new PSViewTokenMgr(this);
  error_mgr_ = new PSViewErrorMgr(this);

  mark_token_ = new PSViewMarkToken(this);

  operator_mgr_  ->init();
  dictionary_mgr_->init();

  init();
}

CPSView::
~CPSView()
{
  term();
}

void
CPSView::
setDebug(bool flag)
{
  debug_ = flag;
}

void
CPSView::
setExitFlag(bool flag)
{
  exit_flag_ = flag;
}

void
CPSView::
init()
{
  start_time_ = time(NULL);

  getErrorMgr()->setError(false);

  exit_flag_ = false;

  getMemoryMgr     ()->init();
  getGStateMgr     ()->init();
  getGStateTokenMgr()->init();
  getResourceMgr   ()->init();
}

void
CPSView::
term()
{
}

void
CPSView::
setRenderer(CPSViewRenderer *renderer)
{
  graphics_ = renderer;
}

void
CPSView::
openCurrentFile(const std::string &filename)
{
  auto *token = new PSViewTextFileToken(this, filename, "r");

  if (token == NULL) {
    getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_FILENAME);
    return;
  }

  if (current_file_ != NULL)
    current_file_stack_.push_back(current_file_);

  current_file_ = token;
}

void
CPSView::
closeCurrentFile()
{
  if (current_file_ != NULL)
    current_file_->close();

  /* Print any remaining Stack Tokens */

  if (getDebug()) {
    CStrUtil::printf("Stack : ");
    operand_stack_->print();
    CStrUtil::printf("\n");
  }

  if (current_file_stack_.size() > 0) {
    current_file_ = current_file_stack_[current_file_stack_.size() - 1];

    current_file_stack_.pop_back();
  }
  else
    current_file_ = NULL;
}

void
CPSView::
setCurrentFile(PSViewFileToken *token)
{
  current_file_ = token;
}

PSViewFileToken *
CPSView::
getCurrentFile()
{
  return current_file_;
}

void
CPSView::
executeCurrentFile()
{
  if (current_file_ == NULL)
    return;

  /* Check First Line */

  if (! current_file_->initPostScript()) {
    CStrUtil::eprintf("PSView: Invalid PostScript File %s\n",
                      current_file_->getFileName().c_str());

    return;
  }

  /* Read and Execute each Token in the File */

  PSViewToken *token;

  while ((token = current_file_->readToken()) != NULL) {
    if (token->isProcedure())
      operand_stack_->push(token);
    else
      token->execute();

    if (current_file_ == NULL || getErrorMgr()->getError())
      break;
  }
}

void
CPSView::
executive()
{
  PSViewOperatorMgr::executiveOp(getOperatorMgr());
}

bool
CPSView::
getLine(std::string &line)
{
  CReadLine readline;

  line = readline.readLine();

  if (readline.eof())
    return false;

  return true;
}
