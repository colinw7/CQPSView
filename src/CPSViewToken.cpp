#include <CPSViewI.h>

const char * const ARRAY_TYPE_NAME        = "arraytype";
const char * const BOOLEAN_TYPE_NAME      = "booleantype";
const char * const DICT_TYPE_NAME         = "dicttype";
const char * const FILE_TYPE_NAME         = "filetype";
const char * const FONT_TYPE_NAME         = "fonttype";
const char * const GSTATE_TYPE_NAME       = "gstatetype";
const char * const INTEGER_TYPE_NAME      = "integertype";
const char * const MARK_TYPE_NAME         = "marktype";
const char * const NAME_TYPE_NAME         = "nametype";
const char * const NULL_TYPE_NAME         = "nulltype";
const char * const OPERATOR_TYPE_NAME     = "operatortype";
const char * const PACKED_ARRAY_TYPE_NAME = "packedarraytype";
const char * const REAL_TYPE_NAME         = "realtype";
const char * const SAVE_TYPE_NAME         = "savetype";
const char * const STRING_TYPE_NAME       = "stringtype";

PSViewTokenMgr::
PSViewTokenMgr(CPSView *psview) :
 psview_            (psview),
 packing_           (false),
 last_execute_token_(NULL)
{
}

void
PSViewTokenMgr::
setPacking(int flag)
{
  packing_ = flag;
}

void
PSViewTokenMgr::
setLastExecuteToken(PSViewToken *token)
{
  last_execute_token_ = token;
}

const PSViewName &
PSViewTokenMgr::
getLBraceName()
{
  return psview_->getNameMgr()->getName("{");
}

const PSViewName &
PSViewTokenMgr::
getRBraceName()
{
  return psview_->getNameMgr()->getName("}");
}

//-----------

PSViewToken::
PSViewToken(CPSView *psview, PSViewTokenType type, PSViewTokenCompositeType composite,
            PSViewTokenExecutableType executable, PSViewTokenAccessType access) :
 psview_    (psview),
 type_      (type),
 composite_ (composite),
 memory_    (NULL),
 executable_(executable),
 access_    (access)
{
}

PSViewToken::
PSViewToken(const PSViewToken &token) :
 psview_    (token.psview_),
 type_      (token.type_),
 composite_ (token.composite_),
 memory_    (NULL),
 executable_(token.executable_),
 access_    (token.access_)
{
}

PSViewToken::
~PSViewToken()
{
}

PSViewToken &
PSViewToken::
operator=(const PSViewToken &token)
{
  type_       = token.type_;
  composite_  = token.composite_;
  memory_     = NULL;
  executable_ = token.executable_;
  access_     = token.access_;

  return *this;
}

PSVboolean
PSViewToken::
isBoolean()
{
  return isType(PSVIEW_TOKEN_TYPE_BOOLEAN);
}

PSVboolean
PSViewToken::
isFontId()
{
  return isType(PSVIEW_TOKEN_TYPE_FONT_ID);
}

PSVboolean
PSViewToken::
isInteger()
{
  return isType(PSVIEW_TOKEN_TYPE_INTEGER);
}

PSVboolean
PSViewToken::
isName()
{
  return isType(PSVIEW_TOKEN_TYPE_NAME);
}

PSVboolean
PSViewToken::
isMark()
{
  return isType(PSVIEW_TOKEN_TYPE_MARK);
}

PSVboolean
PSViewToken::
isNull()
{
  return isType(PSVIEW_TOKEN_TYPE_NULL);
}

PSVboolean
PSViewToken::
isOperator()
{
  return isType(PSVIEW_TOKEN_TYPE_OPERATOR);
}

PSVboolean
PSViewToken::
isReal()
{
  return isType(PSVIEW_TOKEN_TYPE_REAL);
}

PSVboolean
PSViewToken::
isSave()
{
  return isType(PSVIEW_TOKEN_TYPE_SAVE);
}

PSVboolean
PSViewToken::
isComposite()
{
  return composite_ == PSVIEW_TOKEN_COMPOSITE;
}

PSVboolean
PSViewToken::
isArray()
{
  return isType(PSVIEW_TOKEN_TYPE_ARRAY);
}

PSVboolean
PSViewToken::
isDictionary()
{
  return isType(PSVIEW_TOKEN_TYPE_DICTIONARY);
}

PSVboolean
PSViewToken::
isFile()
{
  return isType(PSVIEW_TOKEN_TYPE_FILE);
}

PSVboolean
PSViewToken::
isGState()
{
  return isType(PSVIEW_TOKEN_TYPE_GSTATE);
}

PSVboolean
PSViewToken::
isPackedArray()
{
  return isType(PSVIEW_TOKEN_TYPE_PACKED_ARRAY);
}

PSVboolean
PSViewToken::
isString()
{
  return isType(PSVIEW_TOKEN_TYPE_STRING);
}

PSVboolean
PSViewToken::
isNumber()
{
  return isType(PSVIEW_TOKEN_TYPE_INTEGER) ||
         isType(PSVIEW_TOKEN_TYPE_REAL);
}

PSVboolean
PSViewToken::
isProcedure()
{
  return (isExecutable() &&
          (isType(PSVIEW_TOKEN_TYPE_ARRAY) ||
           isType(PSVIEW_TOKEN_TYPE_PACKED_ARRAY)));
}

PSVboolean
PSViewToken::
getBooleanValue()
{
  if      (type_ == PSVIEW_TOKEN_TYPE_BOOLEAN) {
    PSViewBooleanToken *boolean_token = dynamic_cast<PSViewBooleanToken *>(this);

    return boolean_token->getValue();
  }
  else {
    CTHROW("No boolean value for token");

    return false;
  }
}

PSVinteger
PSViewToken::
getIntegerValue()
{
  if      (type_ == PSVIEW_TOKEN_TYPE_INTEGER) {
    PSViewIntegerToken *integer_token = dynamic_cast<PSViewIntegerToken *>(this);

    return integer_token->getValue();
  }
  else if (type_ == PSVIEW_TOKEN_TYPE_REAL) {
    PSViewRealToken *real_token = dynamic_cast<PSViewRealToken *>(this);

    return (PSVinteger) real_token->getValue();
  }
  else {
    CTHROW("No integer value for token");

    return 0;
  }
}

PSVreal
PSViewToken::
getRealValue()
{
  if      (type_ == PSVIEW_TOKEN_TYPE_INTEGER) {
    PSViewIntegerToken *integer_token = dynamic_cast<PSViewIntegerToken *>(this);

    return (PSVreal) integer_token->getValue();
  }
  else if (type_ == PSVIEW_TOKEN_TYPE_REAL) {
    PSViewRealToken *real_token = dynamic_cast<PSViewRealToken *>(this);

    return real_token->getValue();
  }
  else {
    CTHROW("No real value for token");

    return 0.0;
  }
}

PSViewToken *
PSViewToken::
toName()
{
  PSViewToken *token1 = new PSViewNameToken(psview_, getTypeName());

  return token1;
}

string
PSViewToken::
getTypeName()
{
  if      (type_ == PSVIEW_TOKEN_TYPE_ARRAY)
    return ARRAY_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_BOOLEAN)
    return BOOLEAN_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_DICTIONARY)
    return DICT_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_FILE)
    return FILE_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_FONT_ID)
    return FONT_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_GSTATE)
    return GSTATE_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_INTEGER)
    return INTEGER_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_MARK)
    return MARK_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_NAME)
    return NAME_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_NULL)
    return NULL_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_OPERATOR)
    return OPERATOR_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_PACKED_ARRAY)
    return PACKED_ARRAY_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_REAL)
    return REAL_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_SAVE)
    return SAVE_TYPE_NAME;
  else if (type_ == PSVIEW_TOKEN_TYPE_STRING)
    return STRING_TYPE_NAME;
  else {
    CTHROW("No Name for Token");

    return "";
  }
}

int
PSViewToken::
compare(PSViewToken *token)
{
  if (type_ == token->type_) {
    if      (type_ == PSVIEW_TOKEN_TYPE_BOOLEAN)
      return (dynamic_cast<PSViewBooleanToken *>(this))->
                compare(dynamic_cast<PSViewBooleanToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_FONT_ID)
      return (dynamic_cast<PSViewFontIdToken *>(this))->
                compare(dynamic_cast<PSViewFontIdToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_INTEGER)
      return (dynamic_cast<PSViewIntegerToken *>(this))->
                compare(dynamic_cast<PSViewIntegerToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_MARK)
      return (dynamic_cast<PSViewMarkToken *>(this))->
                compare(dynamic_cast<PSViewMarkToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_NAME)
      return (dynamic_cast<PSViewNameToken *>(this))->
                compare(dynamic_cast<PSViewNameToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_NULL)
      return (dynamic_cast<PSViewNullToken *>(this))->
                compare(dynamic_cast<PSViewNullToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_OPERATOR)
      return (dynamic_cast<PSViewOperatorToken *>(this))->
                compare(dynamic_cast<PSViewOperatorToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_REAL)
      return (dynamic_cast<PSViewRealToken *>(this))->
                compare(dynamic_cast<PSViewRealToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_SAVE)
      return (dynamic_cast<PSViewSaveToken *>(this))->
                compare(dynamic_cast<PSViewSaveToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_ARRAY)
      return (dynamic_cast<PSViewArrayToken *>(this))->
                compare(dynamic_cast<PSViewArrayToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_DICTIONARY)
      return (dynamic_cast<PSViewDictionaryToken *>(this))->
                compare(dynamic_cast<PSViewDictionaryToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_FILE)
      return (dynamic_cast<PSViewFileToken *>(this))->
                compare(dynamic_cast<PSViewFileToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_GSTATE)
      return (dynamic_cast<PSViewGStateToken *>(this))->
                compare(dynamic_cast<PSViewGStateToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_PACKED_ARRAY)
      return (dynamic_cast<PSViewPackedArrayToken *>(this))->
                compare(dynamic_cast<PSViewPackedArrayToken *>(token));
    else if (type_ == PSVIEW_TOKEN_TYPE_STRING)
      return (dynamic_cast<PSViewStringToken *>(this))->
                compare(dynamic_cast<PSViewStringToken *>(token));
    else
      CTHROW("Invalid Type for Compare");
  }

  if (       type_ == PSVIEW_TOKEN_TYPE_INTEGER && token->type_ == PSVIEW_TOKEN_TYPE_REAL)
    return (dynamic_cast<PSViewIntegerToken *>(this))->
              compare(dynamic_cast<PSViewRealToken *>(token));

  if (       type_ == PSVIEW_TOKEN_TYPE_REAL && token->type_ == PSVIEW_TOKEN_TYPE_INTEGER)
    return (dynamic_cast<PSViewRealToken *>(this))->
              compare(dynamic_cast<PSViewIntegerToken *>(token));

  if (       type_ == PSVIEW_TOKEN_TYPE_NAME && token->type_ == PSVIEW_TOKEN_TYPE_STRING)
    return (dynamic_cast<PSViewNameToken *>(this))->
              compare(dynamic_cast<PSViewStringToken *>(token));

  if (       type_ == PSVIEW_TOKEN_TYPE_STRING  && token->type_ == PSVIEW_TOKEN_TYPE_NAME)
    return (dynamic_cast<PSViewStringToken *>(this))->
              compare(dynamic_cast<PSViewNameToken *>(token));

  CTHROW("Invalid Type for Compare");

  return 0;
}

PSViewTokenType
PSViewToken::
getType() const
{
  return type_;
}

PSVboolean
PSViewToken::
isType(PSViewTokenType type) const
{
  return (type_ == type);
}

PSVboolean
PSViewToken::
isExecutable()
{
  return (executable_ == PSVIEW_TOKEN_EXECUTABLE);
}

PSVboolean
PSViewToken::
getWritable()
{
  return access_ == PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE;
}

PSVboolean
PSViewToken::
getReadable()
{
  return (access_ == PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE ||
          access_ == PSVIEW_TOKEN_ACCESS_TYPE_READ_ONLY);
}

PSVboolean
PSViewToken::
getGlobal()
{
  return (isComposite() && psview_->getMemoryMgr()->getGlobal());
}

PSVboolean
PSViewToken::
getLocal()
{
  return (isComposite() && psview_->getMemoryMgr()->getLocal());
}

PSVboolean
PSViewToken::
isLiteral()
{
  return (executable_ == PSVIEW_TOKEN_LITERAL);
}

void
PSViewToken::
setExecutable()
{
  executable_ = PSVIEW_TOKEN_EXECUTABLE;
}

void
PSViewToken::
setLiteral()
{
  executable_ = PSVIEW_TOKEN_LITERAL;
}

void
PSViewToken::
setReadWrite()
{
  access_ = PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE;
}

void
PSViewToken::
setReadOnly()
{
  access_ = PSVIEW_TOKEN_ACCESS_TYPE_READ_ONLY;
}

void
PSViewToken::
setExecuteOnly()
{
  access_ = PSVIEW_TOKEN_ACCESS_TYPE_EXECUTE_ONLY;
}

void
PSViewToken::
setNoAccess()
{
  access_ = PSVIEW_TOKEN_ACCESS_TYPE_NONE;
}

int
PSViewToken::
getMemoryDepth()
{
  if (isComposite())
    return psview_->getMemoryMgr()->getDepth();
  else
    return -1;
}

void
PSViewToken::
execute()
{
  if (psview_->getErrorMgr()->getError())
    return;

  if (isLiteral()) {
    psview_->getOperandStack()->push(this);

    return;
  }

  if (psview_->getDebug()) {
    CStrUtil::printf("Executing ");
    print();
    CStrUtil::printf("\n");

    CStrUtil::printf("Stack     ");
    psview_->getOperandStack()->print();

    CStrUtil::printf("\n");
  }

  psview_->getTokenMgr()->setLastExecuteToken(this);

  executeToken();
}

PSVboolean
PSViewToken::
isMatrix()
{
  if (! isArray() || ! isLiteral())
    return false;

  PSViewArrayToken *array_token = dynamic_cast<PSViewArrayToken *>(this);

  PSVinteger num_tokens = array_token->getNumValues();

  if (num_tokens != 6)
    return false;

  for (int i = 1; i <= 6; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isNumber())
      return false;
  }

  return true;
}

CMatrix2D *
PSViewToken::
getMatrix()
{
  if (! isArray() || ! isLiteral()) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return NULL;
  }

  PSViewArrayToken *array_token = dynamic_cast<PSViewArrayToken *>(this);

  PSVinteger num_tokens = array_token->getNumValues();

  if (num_tokens != 6) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return NULL;
  }

  CMatrix2D *matrix = new CMatrix2D;

  double data[6];

  for (int i = 1; i <= 6; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isNumber()) {
      delete matrix;

      psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return NULL;
    }

    data[i - 1] = sub_token->getRealValue();
  }

  matrix->setValues(data[0], data[2], data[1],
                    data[3], data[4], data[5]);

  return matrix;
}

void
PSViewToken::
setMatrix(CMatrix2D *matrix)
{
  if (! isArray() || ! isLiteral()) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewArrayToken *array_token = dynamic_cast<PSViewArrayToken *>(this);

  double data[6];

  matrix->getValues(&data[0], &data[2], &data[1],
                    &data[3], &data[4], &data[5]);

  for (int i = 1; i <= 6; i++) {
    PSViewToken *sub_token = new PSViewRealToken(psview_, data[i - 1]);

    array_token->setValue(i, sub_token);
  }
}
