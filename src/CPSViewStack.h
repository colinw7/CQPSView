#ifndef CPSViewStack_H
#define CPSViewStack_H

class PSViewOperandStack {
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

 private:
  typedef std::vector<PSViewToken *> TokenStack;

  CPSView    *psview_;
  TokenStack  operand_stack_;
};

//---

class PSViewExecutionStack {
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

 private:
  typedef std::vector<PSViewToken *> TokenStack;

  CPSView    *psview_;
  TokenStack  execution_stack_;
};

//---

class PSViewGraphicsStack {
 public:
  PSViewGraphicsStack(CPSView *psview);

  CPSView *getPSView() const {  return psview_; }

  void push(PSViewGStateToken *token);

  PSViewGStateToken *pop();

  int size();

 private:
  PSViewGraphicsStack(const PSViewGraphicsStack &rhs);
  PSViewGraphicsStack &operator=(const PSViewGraphicsStack &rhs);

 private:
  typedef std::vector<PSViewGStateToken *> TokenStack;

  CPSView    *psview_;
  TokenStack  graphics_stack_;
};

#endif
