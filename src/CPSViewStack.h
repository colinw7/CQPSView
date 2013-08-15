#ifndef PSVIEW_STACK_H
#define PSVIEW_STACK_H

class PSViewOperandStack {
 private:
  typedef std::vector<PSViewToken *> TokenStack;

  CPSView    *psview_;
  TokenStack  operand_stack_;

 public:
  PSViewOperandStack(CPSView *psview);

  CPSView *getPSView() const {  return psview_; }

  void push(PSViewToken *token);

  PSViewToken *pop();

  void exch();
  void roll(int n, int m);

  int size();

  int countToMark();

  PSViewToken *peek(int n);

  void clear();

  void print();

 private:
  PSViewOperandStack(const PSViewOperandStack &rhs);
  PSViewOperandStack &operator=(const PSViewOperandStack &rhs);
};

class PSViewExecutionStack {
 private:
  typedef std::vector<PSViewToken *> TokenStack;

  CPSView    *psview_;
  TokenStack  execution_stack_;

 public:
  PSViewExecutionStack(CPSView *psview);

  CPSView *getPSView() const {  return psview_; }

  void push(PSViewToken *token);

  PSViewToken *pop();

  void popToExit();

  int size();

  int countToMark();

  PSViewToken *peek(int n);

  void clear();

  void print();

 private:
  PSViewExecutionStack(const PSViewExecutionStack &rhs);
  PSViewExecutionStack &operator=(const PSViewExecutionStack &rhs);
};

class PSViewGraphicsStack {
 private:
  typedef std::vector<PSViewGStateToken *> TokenStack;

  CPSView    *psview_;
  TokenStack  graphics_stack_;

 public:
  PSViewGraphicsStack(CPSView *psview);

  CPSView *getPSView() const {  return psview_; }

  void push(PSViewGStateToken *token);

  PSViewGStateToken *pop();

  int size();

 private:
  PSViewGraphicsStack(const PSViewGraphicsStack &rhs);
  PSViewGraphicsStack &operator=(const PSViewGraphicsStack &rhs);
};

#endif
