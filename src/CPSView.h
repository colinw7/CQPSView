#ifndef CPSView_H
#define CPSView_H

#include <CAutoPtr.h>
#include <CPen.h>
#include <CBrush.h>
#include <CPoint2D.h>
#include <CMatrix2D.h>

class CPSViewRenderer {
 public:
  CPSViewRenderer() { }

  virtual ~CPSViewRenderer() { }

  virtual CPSViewRenderer *dup() = 0;

  virtual void clear(const CRGBA &bg) = 0;

  // Pen, Brush
  virtual void setPen(const CPen &pen) = 0;
  virtual void setBrush(const CBrush &brush) = 0;

  virtual void pathInit() = 0;
  virtual void pathMoveTo(const CPoint2D &p) = 0;
  virtual void pathLineTo(const CPoint2D &p) = 0;
  virtual void pathArc(const CPoint2D &, double, double, double, double) = 0;
  virtual void pathBezier2To(const CPoint2D &, const CPoint2D &) = 0;
  virtual void pathBezier3To(const CPoint2D &, const CPoint2D &, const CPoint2D &) = 0;
  virtual void pathClose() = 0;
  virtual void pathStroke() = 0;
  virtual void pathFill() = 0;
  virtual void pathClip() = 0;
};

//---

class PSViewMemoryMgr;
class PSViewResourceMgr;
class PSViewNameMgr;
class PSViewOperatorMgr;
class PSViewDictionaryMgr;
class PSViewOperandStack;
class PSViewExecutionStack;
class PSViewGraphicsStack;
class PSViewGStateMgr;
class PSViewGStateTokenMgr;
class PSViewTokenMgr;
class PSViewErrorMgr;
class PSViewMarkToken;
class PSViewFileToken;

class CPSView {
 public:
  // 8.5 by 11 inches at 72 pixels per inch
  enum { PAGE_WIDTH  = 612 };
  enum { PAGE_HEIGHT = 792 };

 public:
  CPSView();
 ~CPSView();

  time_t getStartTime() { return start_time_; }

  bool getDebug() const { return debug_; }
  void setDebug(bool flag);

  bool getExitFlag() const { return exit_flag_; }
  void setExitFlag(bool flag);

  PSViewMemoryMgr   *getMemoryMgr  () const { return memory_mgr_; }
  PSViewNameMgr     *getNameMgr    () const { return name_mgr_; }
  PSViewResourceMgr *getResourceMgr() const { return resource_mgr_; }

  PSViewDictionaryMgr *getDictionaryMgr() const { return dictionary_mgr_; }

  PSViewOperatorMgr *getOperatorMgr() const { return operator_mgr_; }

  PSViewOperandStack   *getOperandStack  () const { return operand_stack_  ; }
  PSViewExecutionStack *getExecutionStack() const { return execution_stack_; }
  PSViewGraphicsStack  *getGraphicsStack () const { return graphics_stack_ ; }

  CPSViewRenderer *getGraphics() const { return graphics_; }

  PSViewGStateMgr      *getGStateMgr     () const { return gstate_mgr_; }
  PSViewGStateTokenMgr *getGStateTokenMgr() const { return gstate_token_mgr_; }

  PSViewTokenMgr *getTokenMgr() const { return token_mgr_; }

  PSViewErrorMgr *getErrorMgr() const { return error_mgr_; }

  PSViewMarkToken *getMarkToken() const { return mark_token_; }

  void init();
  void term();

  void setRenderer(CPSViewRenderer *renderer);

  void openCurrentFile(const std::string &filename);
  void closeCurrentFile();
  void executeCurrentFile();

  void executive();

  bool getLine(std::string &line);

  void             setCurrentFile(PSViewFileToken *token);
  PSViewFileToken *getCurrentFile();

 private:
  CPSView(const CPSView &rhs);
  CPSView &operator=(const CPSView &rhs);

 private:
  typedef std::vector<PSViewFileToken *> FileTokenStack;

  time_t                         start_time_;
  bool                           debug_;
  bool                           exit_flag_;

  CAutoPtr<PSViewMemoryMgr>      memory_mgr_;
  CAutoPtr<PSViewNameMgr>        name_mgr_;
  CAutoPtr<PSViewResourceMgr>    resource_mgr_;
  CAutoPtr<PSViewDictionaryMgr>  dictionary_mgr_;
  CAutoPtr<PSViewOperatorMgr>    operator_mgr_;
  CAutoPtr<PSViewOperandStack>   operand_stack_;
  CAutoPtr<PSViewExecutionStack> execution_stack_;
  CAutoPtr<PSViewGraphicsStack>  graphics_stack_;
  CPSViewRenderer*               graphics_;
  CAutoPtr<PSViewGStateMgr>      gstate_mgr_;
  CAutoPtr<PSViewGStateTokenMgr> gstate_token_mgr_;
  CAutoPtr<PSViewTokenMgr>       token_mgr_;
  CAutoPtr<PSViewErrorMgr>       error_mgr_;
  CAutoPtr<PSViewMarkToken>      mark_token_;

  PSViewFileToken*               current_file_;
  FileTokenStack                 current_file_stack_;
};

#endif
