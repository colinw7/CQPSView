#include <CPSViewI.h>

PSViewOperandStack::
PSViewOperandStack(CPSView *psview) :
 psview_       (psview),
 operand_stack_()
{
}

void
PSViewOperandStack::
push(PSViewToken *token)
{
  if (token == NULL)
    CTHROW("NULL Token");

  operand_stack_.push_back(token);

  if (psview_->getDebug()) {
    CStrUtil::printf("Stacking ");
    token->print();
    CStrUtil::printf("\n");

    CStrUtil::printf("Stack    ");
    print();

    CStrUtil::printf("\n");
  }
}

PSViewToken *
PSViewOperandStack::
pop()
{
  if (psview_->getErrorMgr()->getError())
    return NULL;

  auto num_stack = operand_stack_.size();

  if (num_stack == 0) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_STACK_UNDERFLOW);
    return NULL;
  }

  PSViewToken *token = operand_stack_[num_stack - 1];

  operand_stack_.pop_back();

  return token;
}

void
PSViewOperandStack::
exch()
{
  auto num_stack = operand_stack_.size();

  if (num_stack < 2) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_STACK_UNDERFLOW);
    return;
  }

  PSViewToken *token1 = operand_stack_[num_stack - 1];
  PSViewToken *token2 = operand_stack_[num_stack - 2];

  operand_stack_.pop_back();
  operand_stack_.pop_back(); // dup

  operand_stack_.push_back(token1);
  operand_stack_.push_back(token2);
}

void
PSViewOperandStack::
roll(int n, int m)
{
  auto num_stack = operand_stack_.size();

  if (int(num_stack) < n) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_STACK_UNDERFLOW);
    return;
  }

  m %= n;

  if (m > 0) {
    for (int i = 0; i < m; i++) {
      auto *token = operand_stack_[num_stack - 1];

      for (int j = 0; j < n - 1; j++)
        operand_stack_[uint(int(num_stack) - j - 1)] = operand_stack_[uint(int(num_stack) - j - 2)];

      operand_stack_[uint(int(num_stack) - n)] = token;
    }
  }
  else {
    for (int i = 0; i < -m; i++) {
      auto *token = operand_stack_[uint(int(num_stack) - n)];

      for (int j = n - 2; j >= 0; j--)
        operand_stack_[uint(int(num_stack) - j - 2)] = operand_stack_[uint(int(num_stack) - j - 1)];

      operand_stack_[uint(int(num_stack) - 1)] = token;
    }
  }
}

int
PSViewOperandStack::
size()
{
  auto num_stack = operand_stack_.size();

  return int(num_stack);
}

int
PSViewOperandStack::
countToMark()
{
  auto num_stack = operand_stack_.size();

  for (int i = int(num_stack - 1); i >= 0; i--) {
    auto *token = operand_stack_[uint(i)];

    if (token->isMark())
      return int(num_stack) - i - 1;
  }

  return -1;
}

PSViewToken *
PSViewOperandStack::
peek(int n)
{
  if (n <= 0) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return NULL;
  }

  auto num_stack = operand_stack_.size();

  if (n > int(num_stack)) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_STACK_UNDERFLOW);
    return NULL;
  }

  auto *token = operand_stack_[uint(n - 1)];

  return token;
}

void
PSViewOperandStack::
clear()
{
  operand_stack_.clear();
}

void
PSViewOperandStack::
print()
{
  auto num_stack = operand_stack_.size();

  for (uint i = 0; i < num_stack; i++) {
    if (i > 0)
      CStrUtil::printf(" ");

    PSViewToken *token = operand_stack_[i];

    token->print();
  }
}

//-----------------

PSViewExecutionStack::
PSViewExecutionStack(CPSView *psview) :
 psview_         (psview),
 execution_stack_()
{
}

void
PSViewExecutionStack::
push(PSViewToken *token)
{
  if (token == NULL)
    CTHROW("NULL Token");

  execution_stack_.push_back(token);
}

PSViewToken *
PSViewExecutionStack::
pop()
{
  auto num_stack = execution_stack_.size();

  if (num_stack == 0) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_STACK_UNDERFLOW);
    return NULL;
  }

  PSViewToken *token = execution_stack_[num_stack - 1];

  execution_stack_.pop_back();

  return token;
}

void
PSViewExecutionStack::
popToExit()
{
  auto num_stack = execution_stack_.size();

  while (num_stack > 0) {
    PSViewToken *token = execution_stack_[num_stack - 1];

    execution_stack_.pop_back();

    if (token->isOperator()) {
      auto *operator_token = static_cast<PSViewOperatorToken *>(token);

      PSViewOperator *opr = operator_token->getValue();

      if (opr->getString() == "exit")
        break;
    }

    num_stack = execution_stack_.size();
  }
}

int
PSViewExecutionStack::
size()
{
  auto num_stack = execution_stack_.size();

  return int(num_stack);
}

int
PSViewExecutionStack::
countToMark()
{
  auto num_stack = execution_stack_.size();

  for (int i = int(num_stack - 1); i >= 0; i--) {
    auto *token = execution_stack_[uint(i)];

    if (token->isMark())
      return int(num_stack) - i - 1;
  }

  return -1;
}

PSViewToken *
PSViewExecutionStack::
peek(int n)
{
  if (n <= 0) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return NULL;
  }

  auto num_stack = execution_stack_.size();

  if (n > int(num_stack)) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_STACK_UNDERFLOW);
    return NULL;
  }

  auto *token = execution_stack_[uint(n - 1)];

  return token;
}

void
PSViewExecutionStack::
clear()
{
  execution_stack_.clear();
}

void
PSViewExecutionStack::
print()
{
  auto num_stack = execution_stack_.size();

  for (uint i = 0; i < num_stack; i++) {
    auto *token = execution_stack_[i];

    token->print();
  }
}

//-----------------

PSViewGraphicsStack::
PSViewGraphicsStack(CPSView *psview) :
 psview_        (psview),
 graphics_stack_()
{
}

void
PSViewGraphicsStack::
push(PSViewGStateToken *token)
{
  if (token == NULL)
    CTHROW("NULL Token");

  graphics_stack_.push_back(token);
}

PSViewGStateToken *
PSViewGraphicsStack::
pop()
{
  auto num_stack = graphics_stack_.size();

  if (num_stack == 0) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_STACK_UNDERFLOW);
    return NULL;
  }

  PSViewGStateToken *token = graphics_stack_[num_stack - 1];

  graphics_stack_.pop_back();

  return token;
}

int
PSViewGraphicsStack::
size()
{
  auto num_stack = graphics_stack_.size();

  return int(num_stack);
}
