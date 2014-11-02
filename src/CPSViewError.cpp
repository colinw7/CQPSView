#include <CPSViewI.h>

string
PSViewErrorMgr::
error_names_[] = {
  "configurationerror",
  "dictfull",
  "dictstackoverflow",
  "dictstackunderflow",
  "execstackoverflow",
  "handleerror",
  "interrupt",
  "invalidaccess",
  "invalidexit",
  "invalidfileaccess",
  "invalidfont",
  "invalidrestore",
  "ioerror",
  "limitcheck",
  "nocurrentpoint",
  "rangecheck",
  "stackoverflow",
  "stackunderflow",
  "syntaxerror",
  "timeout",
  "typecheck",
  "undefined",
  "undefinedfilename",
  "undefinedresource",
  "undefinedresult",
  "unmatchedmark",
  "unregistered",
};

PSViewErrorMgr::
PSViewErrorMgr(CPSView *psview) :
 psview_     (psview),
 error_token_(NULL),
 error_name_ (NULL),
 error_flag_ (false)
{
}

PSViewErrorMgr::
~PSViewErrorMgr()
{
}

void
PSViewErrorMgr::
raise(PSViewErrorType type)
{
  if (CEnvInst.exists("PSVIEW_ERROR_EXIT")) {
    cerr << error_names_[type] << endl;
    exit(1);
  }

  if (CEnvInst.exists("PSVIEW_ERROR_SEGV")) {
    cerr << error_names_[type] << endl;
    ::raise(SIGSEGV);
  }

  error_token_ = getPSView()->getTokenMgr()->getLastExecuteToken();

  if (error_token_ != NULL)
    getPSView()->getOperandStack()->push(error_token_);

  PSViewToken *value = getPSView()->getDictionaryMgr()->lookup("errordict");

  if (! value->isDictionary())
    CTHROW("Bad errordict");

  PSViewDictionaryToken *error_dict = (PSViewDictionaryToken *) value;

  error_name_ = new PSViewNameToken(getPSView(), error_names_[type]);

  PSViewToken *error_handler = error_dict->getValue(error_name_);

  if (error_handler->isNull())
    defaultErrorHandler();
  else
    error_handler->execute();
}

void
PSViewErrorMgr::
defaultErrorHandler()
{
  static bool inErrorHandler;

  if (inErrorHandler)
    return;

  inErrorHandler = true;

  /*--------------*/

  PSViewToken *value = getPSView()->getDictionaryMgr()->lookup("$error");

  if (! value->isDictionary())
    CTHROW("Bad $error");

  PSViewDictionaryToken *error_dict = (PSViewDictionaryToken *) value;

  /*--------------*/

  value = new PSViewBooleanToken(getPSView(), true);

  error_dict->addValue("newerror", value);

  /*--------------*/

  error_dict->addValue("errorname", error_name_);

  /*--------------*/

  error_dict->addValue("command", error_token_);

  /*--------------*/

  value = new PSViewArrayToken(getPSView(), 1);

  error_dict->addValue("errorinfo", value);

  /*--------------*/

  int num = getPSView()->getOperandStack()->size();

  PSViewArrayToken *array_token = new PSViewArrayToken(getPSView(), num);

  for (int i = 1; i <= num; i++)
    array_token->setValue(i, getPSView()->getOperandStack()->peek(i));

  error_dict->addValue("ostack", array_token);

  /*--------------*/

  num = getPSView()->getExecutionStack()->size();

  array_token = new PSViewArrayToken(getPSView(), num);

  for (int i = 1; i <= num; i++)
    array_token->setValue(i, getPSView()->getExecutionStack()->peek(i));

  error_dict->addValue("estack", array_token);

  /*--------------*/

  num = getPSView()->getDictionaryMgr()->getNumDictionaries();

  array_token = new PSViewArrayToken(getPSView(), num);

  for (int i = 1; i <= num; i++)
    array_token->setValue(i, getPSView()->getDictionaryMgr()->getDictionary(i));

  error_dict->addValue("dstack", array_token);

  /*--------------*/

  value = new PSViewBooleanToken(getPSView(), true);

  error_dict->addValue("recordstack", value);

  /*--------------*/

  value = new PSViewBooleanToken(getPSView(), false);

  error_dict->addValue("binary", value);

  /*--------------*/

  value = getPSView()->getDictionaryMgr()->lookup("stop");

  value->execute();

  /*--------------*/

  inErrorHandler = false;
}

void
PSViewErrorMgr::
defaultHandleError()
{
  CStrUtil::eprintf("PSView: ");

  /*--------------*/

  PSViewToken *value = getPSView()->getDictionaryMgr()->lookup("$error");

  if (! value->isDictionary())
    CTHROW("Bad $error");

  PSViewDictionaryToken *error_dict = (PSViewDictionaryToken *) value;

  /*--------------*/

  value = error_dict->getValue("errorname");

  value->print();

  /*--------------*/

  CStrUtil::eprintf("  Last Executed ");

  /*--------------*/

  value = error_dict->getValue("command");

  value->print();

  /*--------------*/

  value = new PSViewBooleanToken(getPSView(), false);

  error_dict->addValue("newerror", value);
}

void
PSViewErrorMgr::
setError(bool flag)
{
  error_flag_ = flag;
}

bool
PSViewErrorMgr::
getError()
{
  return error_flag_;
}
