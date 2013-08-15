#ifndef PSVIEW_TOKEN_H
#define PSVIEW_TOKEN_H

enum PSViewTokenType {
  PSVIEW_TOKEN_TYPE_NONE,
  PSVIEW_TOKEN_TYPE_BOOLEAN,
  PSVIEW_TOKEN_TYPE_FONT_ID,
  PSVIEW_TOKEN_TYPE_INTEGER,
  PSVIEW_TOKEN_TYPE_MARK,
  PSVIEW_TOKEN_TYPE_NAME,
  PSVIEW_TOKEN_TYPE_NULL,
  PSVIEW_TOKEN_TYPE_OPERATOR,
  PSVIEW_TOKEN_TYPE_REAL,
  PSVIEW_TOKEN_TYPE_SAVE,
  PSVIEW_TOKEN_TYPE_ARRAY,
  PSVIEW_TOKEN_TYPE_DICTIONARY,
  PSVIEW_TOKEN_TYPE_FILE,
  PSVIEW_TOKEN_TYPE_GSTATE,
  PSVIEW_TOKEN_TYPE_PACKED_ARRAY,
  PSVIEW_TOKEN_TYPE_STRING
};

enum PSViewTokenAccessType {
  PSVIEW_TOKEN_ACCESS_TYPE_NONE,
  PSVIEW_TOKEN_ACCESS_TYPE_READ_ONLY,
  PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE,
  PSVIEW_TOKEN_ACCESS_TYPE_EXECUTE_ONLY
};

enum PSViewTokenCompositeType {
  PSVIEW_TOKEN_COMPOSITE,
  PSVIEW_TOKEN_NON_COMPOSITE
};

enum PSViewTokenExecutableType {
  PSVIEW_TOKEN_EXECUTABLE,
  PSVIEW_TOKEN_LITERAL
};

class PSViewTokenMgr {
 private:
  CPSView     *psview_;
  bool         packing_;
  PSViewToken *last_execute_token_;

 public:
  PSViewTokenMgr(CPSView *psview);

  CPSView *getPSView() const { return psview_; }

  int getPacking() const { return packing_; }

  void setPacking(int packing);

  PSViewToken *getLastExecuteToken() const { return last_execute_token_; }

  void setLastExecuteToken(PSViewToken *token);

  const PSViewName &getLBraceName();
  const PSViewName &getRBraceName();
};

class PSViewToken {
 protected:
  CPSView                   *psview_;
  PSViewTokenType            type_;
  PSViewTokenCompositeType   composite_;
  PSViewMemory              *memory_;
  PSViewTokenExecutableType  executable_;
  PSViewTokenAccessType      access_;

 protected:
  PSViewToken(CPSView *psview, PSViewTokenType type, PSViewTokenCompositeType composite,
              PSViewTokenExecutableType executable, PSViewTokenAccessType access);

  PSViewToken(const PSViewToken &token);

  PSViewToken &operator=(const PSViewToken &token);

 public:
  // virtual functions

  virtual ~PSViewToken();

  virtual PSViewToken *dup() const = 0;

  virtual int compare(PSViewToken *token);

  virtual void executeToken() = 0;

  virtual const PSViewName &getName() = 0;

  virtual void print() = 0;

  virtual std::string toString() = 0;

  //----

  CPSView *getPSView() const { return psview_; }

  PSViewTokenType getType() const;

  PSVboolean isType(PSViewTokenType type) const;

  PSVboolean isExecutable();
  PSVboolean getWritable();
  PSVboolean getReadable();
  PSVboolean getGlobal();
  PSVboolean getLocal();
  PSVboolean isLiteral();

  void setExecutable();
  void setLiteral();
  void setReadWrite();
  void setReadOnly();
  void setExecuteOnly();
  void setNoAccess();

  int getMemoryDepth();

  PSVboolean isBoolean();
  PSVboolean isFontId();
  PSVboolean isInteger();
  PSVboolean isName();
  PSVboolean isMark();
  PSVboolean isNull();
  PSVboolean isOperator();
  PSVboolean isReal();
  PSVboolean isSave();
  PSVboolean isComposite();
  PSVboolean isArray();
  PSVboolean isDictionary();
  PSVboolean isFile();
  PSVboolean isGState();
  PSVboolean isPackedArray();
  PSVboolean isString();
  PSVboolean isNumber();
  PSVboolean isProcedure();

  PSVboolean getBooleanValue();
  PSVinteger getIntegerValue();
  PSVreal    getRealValue();

  PSViewToken *toName();

  std::string getTypeName();

  void execute();

  PSVboolean  isMatrix();
  CMatrix2D  *getMatrix();
  void        setMatrix(CMatrix2D *matrix);
};

#endif
