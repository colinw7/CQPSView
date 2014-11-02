#include <CPSViewI.h>

const int MAX_DICTIONARY_SIZE        = 65535;
const int MAX_DICTIONARY_STACK_DEPTH = 20;
const int ERROR_DICTIONARY_SIZE      = 29;
const int SUB_ERROR_DICTIONARY_SIZE  = 10;
const int SYSTEM_DICTIONARY_SIZE     = 500;
const int GLOBAL_DICTIONARY_SIZE     = 200;
const int USER_DICTIONARY_SIZE       = 200;
const int FONT_DIRECTORY_SIZE        = 100;

const int PSVIEW_OPERAND_STACK_SIZE   = 500;
const int PSVIEW_EXECUTION_STACK_SIZE = 250;

const char *
PSViewDictionaryMgr::
standard_encoding_[256] = {
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  "space"         , "exclam"        , "quotedbl"     , "numbersign"    ,
  "dollar"        , "percent"       , "ampersand"    , "quoteright"    ,
  "parenleft"     , "parenright"    , "asterisk"     , "plus"          ,
  "comma"         , "hyphen"        , "period"       , "slash"         ,
  "zero"          , "one"           , "two"          , "three"         ,
  "four"          , "five"          , "six"          , "seven"         ,
  "eight"         , "nine"          , "colon"        , "semicolon"     ,
  "less"          , "equal"         , "greater"      , "question"      ,
  "at"            , "A"             , "B"            , "C"             ,
  "D"             , "E"             , "F"            , "G"             ,
  "H"             , "I"             , "J"            , "K"             ,
  "L"             , "M"             , "N"            , "O"             ,
  "P"             , "Q"             , "R"            , "S"             ,
  "T"             , "U"             , "V"            , "W"             ,
  "X"             , "Y"             , "Z"            , "bracketleft"   ,
  "backslash"     , "bracketright"  , "asciicircum"  , "underscore"    ,
  "quoteleft"     , "a"             , "b"            , "c"             ,
  "d"             , "e"             , "f"            , "g"             ,
  "h"             , "i"             , "j"            , "k"             ,
  "l"             , "m"             , "n"            , "o"             ,
  "p"             , "q"             , "r"            , "s"             ,
  "t"             , "u"             , "v"            , "w"             ,
  "x"             , "y"             , "z"            , "braceleft"     ,
  "bar"           , "braceright"    , "asciitilde"   , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , "exclamdown"    , "cent"         , "sterling"      ,
  "fraction"      , "yen"           , "florin"       , "section"       ,
  "currency"      , "quotesingle"   , "quotedblleft" , "guillemotleft" ,
  "guilsinglleft" , "guilsinglright", "fi"           , "fl"            ,
  NULL            , "endash"        , "dagger"       , "daggerdbl"     ,
  "periodcentered", NULL            , "paragraph"    , "bullet"        ,
  "quotesinglbase", "quotedblbase"  , "quotedblright", "guillemotright",
  "ellipsis"      , "perthousand"   , NULL           , "questiondown"  ,
  NULL            , "grave"         , "acute"        , "circumflex"    ,
  "tilde"         , "macron"        , "breve"        , "dotaccent"     ,
  "dieresis"      , NULL            , "ring"         , "cedilla"       ,
  NULL            , "hungarumlaut"  , "ogonek"       , "caron"         ,
  "emdash"        , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , "AE"            , NULL           , "ordfeminine"   ,
  NULL            , NULL            , NULL           , NULL            ,
  "Lslash"        , "Oslash"        , "OE"           , "ordmasculine"  ,
  NULL            , NULL            , NULL           , NULL            ,
  NULL            , "ae"            , NULL           , NULL            ,
  NULL            , "dotlessi"      , NULL           , NULL            ,
  "lslash"        , "oslash"        , "oe"           , "germandbls"    ,
  NULL            , NULL            , NULL           , NULL            ,
};

const char *
PSViewDictionaryMgr::
error_names_[] = {
  "$error"            ,
  "configurationerror",
  "dictfull"          ,
  "dictstackoverflow" ,
  "dictstackunderflow",
  "execstackoverflow" ,
  "handleerror"       ,
  "interrupt"         ,
  "invalidaccess"     ,
  "invalidexit"       ,
  "invalidfileaccess" ,
  "invalidfont"       ,
  "invalidrestore"    ,
  "ioerror"           ,
  "limitcheck"        ,
  "nocurrentpoint"    ,
  "rangecheck"        ,
  "stackoverflow"     ,
  "stackunderflow"    ,
  "syntaxerror"       ,
  "timeout"           ,
  "typecheck"         ,
  "undefined"         ,
  "undefinedfilename" ,
  "undefinedresource" ,
  "undefinedresult"   ,
  "unmatchedmark"     ,
  "unregistered"      ,
  "VMerror"           ,
  NULL                ,
};

PSViewDictionaryMgr::
PSViewDictionaryMgr(CPSView *psview) :
 psview_              (psview),
 error_dictionary_    (),
 sub_error_dictionary_(),
 internal_dictionary_ (),
 dictionary_stack_    ()
{
}

PSViewDictionaryMgr::
~PSViewDictionaryMgr()
{
}

void
PSViewDictionaryMgr::
init()
{
  dictionary_stack_.clear();

  //------

  createError();

  //------

  PSViewDictionaryToken *dictionary = createSystem();

  beginDictionary(dictionary);

  //------

  dictionary = createGlobal();

  beginDictionary(dictionary);

  //------

  dictionary = createUser();

  beginDictionary(dictionary);

  //------

  internal_dictionary_ = new PSViewDictionaryToken(this, 100);
}

void
PSViewDictionaryMgr::
createError()
{
  error_dictionary_ = new PSViewDictionaryToken(this, ERROR_DICTIONARY_SIZE);

  //------

  for (int i = 0; error_names_[i] != NULL; ++i) {
    PSViewToken *value = new PSViewNullToken(getPSView());

    error_dictionary_->addValue(error_names_[i], value);
  }

  //------

  sub_error_dictionary_ = new PSViewDictionaryToken(this, SUB_ERROR_DICTIONARY_SIZE);

  PSViewToken *value = new PSViewBooleanToken(getPSView(), false);

  sub_error_dictionary_->addValue("newerror", value);

  value = new PSViewNameToken(getPSView(), "");

  sub_error_dictionary_->addValue("errorname", value);

  value = new PSViewNullToken(getPSView());

  sub_error_dictionary_->addValue("command", value);

  value = new PSViewArrayToken(getPSView(), 1);

  sub_error_dictionary_->addValue("errorinfo", value);

  value = new PSViewArrayToken(getPSView(), 1);

  sub_error_dictionary_->addValue("ostack", value);

  value = new PSViewArrayToken(getPSView(), 1);

  sub_error_dictionary_->addValue("estack", value);

  value = new PSViewArrayToken(getPSView(), 1);

  sub_error_dictionary_->addValue("dstack", value);

  value = new PSViewBooleanToken(getPSView(), true);

  sub_error_dictionary_->addValue("recordstacks", value);

  value = new PSViewBooleanToken(getPSView(), false);

  sub_error_dictionary_->addValue("binary", value);
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
createSystem()
{
  PSViewDictionaryToken *dictionary = new PSViewDictionaryToken(this, SYSTEM_DICTIONARY_SIZE);

  //------

  getPSView()->getOperatorMgr()->addSystemOperators(dictionary);

  //------

  dictionary->setReadOnly();

  //------

  PSViewToken *value = new PSViewDictionaryToken(this, FONT_DIRECTORY_SIZE);

  dictionary->addValue("FontDirectory", value);

  //------

  PSViewArrayToken *array_token = new PSViewArrayToken(getPSView(), 256);

  dictionary->addValue("StandardEncoding", array_token);

  PSViewToken *sub_value;

  for (int i = 0; i < 256; ++i) {
    if (standard_encoding_[i] != NULL)
      sub_value = new PSViewNameToken(getPSView(), standard_encoding_[i]);
    else
      sub_value = new PSViewNameToken(getPSView(), ".notdef");

    array_token->setValue(i + 1, sub_value);
  }

  //------

  dictionary->addValue("errordict", error_dictionary_);

  dictionary->addValue("$error", sub_error_dictionary_);

  //------

  value = new PSViewDictionaryToken(this, 11);

  dictionary->addValue("statusdict", value);

  //------

  value = new PSViewStringToken(getPSView(), 128);

  dictionary->addValue("=string", value);

  //------

  return dictionary;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
createGlobal()
{
  PSViewDictionaryToken *dictionary = new PSViewDictionaryToken(this, GLOBAL_DICTIONARY_SIZE);

  return dictionary;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
createUser()
{
  PSViewDictionaryToken *dictionary = new PSViewDictionaryToken(this, USER_DICTIONARY_SIZE);

  //------

  PSViewToken *value;

  value = new PSViewIntegerToken(getPSView(), PSVIEW_OPERAND_STACK_SIZE);

  dictionary->addValue("MaxOpStack", value);

  value = new PSViewIntegerToken(getPSView(), MAX_DICTIONARY_STACK_DEPTH);

  dictionary->addValue("MaxDictStack", value);

  value = new PSViewIntegerToken(getPSView(), PSVIEW_EXECUTION_STACK_SIZE);

  dictionary->addValue("MaxExecStack", value);

  return dictionary;
}

int
PSViewDictionaryMgr::
getNumDictionaries()
{
  return dictionary_stack_.size();
}

void
PSViewDictionaryMgr::
addToCurrent(PSViewToken *key, PSViewToken *value)
{
  PSViewDictionaryToken *dictionary = getCurrentDictionary();

  if (! dictionary->getWritable()) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);
    return;
  }

  dictionary->addValue(key, value);
}

PSViewToken *
PSViewDictionaryMgr::
lookup(const string &str)
{
  const PSViewName &name = getPSView()->getNameMgr()->getName(str);

  PSViewToken *token = lookup(name);

  return token;
}

PSViewToken *
PSViewDictionaryMgr::
lookup(PSViewToken *key)
{
  int num = dictionary_stack_.size();

  for (int i = num - 1; i >= 0; --i) {
    PSViewDictionaryToken *dictionary = dictionary_stack_[i];

    PSViewToken *token = dictionary->getValue(key);

    if (token != NULL)
      return token;
  }

  return NULL;
}

PSViewToken *
PSViewDictionaryMgr::
lookup(const PSViewName &name)
{
  int num = dictionary_stack_.size();

  for (int i = num - 1; i >= 0; --i) {
    PSViewDictionaryToken *dictionary = dictionary_stack_[i];

    PSViewToken *token = dictionary->getValue(name);

    if (token != NULL)
      return token;
  }

  return NULL;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
lookupDictionary(const string &str)
{
  const PSViewName &name = getPSView()->getNameMgr()->getName(str);

  PSViewDictionaryToken *dictionary = lookupDictionary(name);

  return dictionary;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
lookupDictionary(const PSViewName &name)
{
  int num = dictionary_stack_.size();

  for (int i = num - 1; i >= 0; --i) {
    PSViewDictionaryToken *dictionary = dictionary_stack_[i];

    PSViewToken *token = dictionary->getValue(name);

    if (token != NULL)
      return dictionary;
  }

  return NULL;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
lookupDictionary(PSViewToken *key)
{
  int num = dictionary_stack_.size();

  for (int i = num - 1; i >= 0; i--) {
    PSViewDictionaryToken *dictionary = dictionary_stack_[i];

    PSViewToken *token = dictionary->getValue(key);

    if (token != NULL)
      return dictionary;
  }

  return NULL;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
getSystemDictionary()
{
  PSViewDictionaryToken *dictionary = dictionary_stack_[0];

  return dictionary;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
getGlobalDictionary()
{
  PSViewDictionaryToken *dictionary = dictionary_stack_[1];

  return dictionary;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
getUserDictionary()
{
  PSViewDictionaryToken *dictionary = dictionary_stack_[2];

  return dictionary;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
getInternalDictionary()
{
  return internal_dictionary_;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
getCurrentDictionary()
{
  PSViewDictionaryToken *dictionary = dictionary_stack_[dictionary_stack_.size() - 1];

  return dictionary;
}

PSViewDictionaryToken *
PSViewDictionaryMgr::
getDictionary(int num)
{
  PSViewDictionaryToken *dictionary = dictionary_stack_[num - 1];

  return dictionary;
}

void
PSViewDictionaryMgr::
beginDictionary(PSViewDictionaryToken *dictionary)
{
  dictionary_stack_.push_back(dictionary);
}

void
PSViewDictionaryMgr::
endDictionary()
{
  int num = dictionary_stack_.size();

  if (num <= 3) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_DICT_STACK_UNDERFLOW);

    return;
  }

  dictionary_stack_.pop_back();
}

void
PSViewDictionaryMgr::
emptyDictionaryStack()
{
  int num = dictionary_stack_.size();

  while (num > 3) {
    dictionary_stack_.pop_back();

    num--;
  }
}

//------------------

PSViewDictionary::
PSViewDictionary(PSViewDictionaryMgr *mgr, PSVinteger max_length) :
 mgr_    (mgr),
 keyvals_(),
 start_  (0),
 end_    (max_length - 1),
 used_   (0)
{
}

PSViewDictionary::
PSViewDictionary(const PSViewDictionary &dictionary) :
 mgr_    (dictionary.mgr_),
 keyvals_(),
 start_  (0),
 end_    (0),
 used_   (0)
{
  int size = dictionary.end_ + 1;

  end_ = size - 1;

  for (int i = 1; i <= dictionary.used_; ++i) {
    PSViewKeyValue *key_value = dictionary.getKeyValue(i);

    PSViewToken *key   = key_value->getKey  ()->dup();
    PSViewToken *value = key_value->getValue()->dup();

    addValue(key, value);
  }
}

PSViewDictionary::
~PSViewDictionary()
{
}

PSViewDictionary *
PSViewDictionary::
dup() const
{
  return new PSViewDictionary(*this);
}

int
PSViewDictionary::
compare(PSViewDictionary *dictionary)
{
  if      (getNumValues() > dictionary->getNumValues())
    return  1;
  else if (getNumValues() < dictionary->getNumValues())
    return -1;

  for (int i = 1; i <= getNumValues(); ++i) {
    int cmp = getKeyValue(i)->compare(dictionary->getKeyValue(i));

    if (cmp != 0)
      return cmp;
  }

  return 0;
}

int
PSViewDictionary::
getNumValues()
{
  return used_;
}

int
PSViewDictionary::
getMaxValues()
{
  return (end_ - start_ + 1);
}

PSViewKeyValue *
PSViewDictionary::
getKeyValue(int i) const
{
  if (i < 1 || i > used_)
    CTHROW("Subscript Error");

  return (PSViewKeyValue *) &keyvals_[start_ + i - 1];
}

PSViewToken *
PSViewDictionary::
getKey(int i)
{
  return getKeyValue(i)->getKey();
}

PSViewToken *
PSViewDictionary::
getValue(int i)
{
  return getKeyValue(i)->getValue();
}

PSViewToken *
PSViewDictionary::
getValue(PSViewToken *key)
{
  PSVinteger i = start_;

  for (PSVinteger j = 0; j < used_; ++j, ++i) {
    PSViewToken *key1 = keyvals_[i].getKey();

    if (key1->compare(key) == 0)
      return keyvals_[i].getValue();
  }

  return NULL;
}

PSViewToken *
PSViewDictionary::
getValue(const PSViewName &name)
{
  PSVinteger i = start_;

  for (PSVinteger j = 0; j < used_; ++j, ++i) {
    PSViewToken *key1 = keyvals_[i].getKey();

    if (key1->getName().compare(name) == 0)
      return keyvals_[i].getValue();
  }

  return NULL;
}

PSViewToken *
PSViewDictionary::
getValue(const string &str)
{
  const PSViewName &name = getPSView()->getNameMgr()->getName(str);

  return getValue(name);
}

void
PSViewDictionary::
setValue(PSVinteger pos, PSViewToken *key, PSViewToken *value)
{
  if (pos < 1 || pos > used_)
    CTHROW("Subscript Error");

  keyvals_[start_ + pos - 1].setKeyValue(key, value);
}

void
PSViewDictionary::
resize()
{
  long num_keyvals = end_ - start_ + 1;

  if (num_keyvals >= MAX_DICTIONARY_SIZE) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_DICT_FULL);
    return;
  }

  long num_keyvals1 = (2*num_keyvals + 1) % MAX_DICTIONARY_SIZE;

  keyvals_.resize(num_keyvals1 + 1);

  start_ = 0;
  end_   = num_keyvals1 - 1;
}

void
PSViewDictionary::
addValue(const string &name, PSViewToken *value)
{
  PSViewToken *key = new PSViewNameToken(getPSView(), name);

  addValue(key, value);
}

void
PSViewDictionary::
addValue(const PSViewName &key, PSViewToken *value)
{
  PSViewToken *key1 = new PSViewNameToken(getPSView(), key);

  addValue(key1, value);
}

void
PSViewDictionary::
addValue(PSViewToken *key, PSViewToken *value)
{
  int i = start_;

  for (int j = 0; j < used_; ++j) {
    if (keyvals_[i].getKey()->compare(key) == 0) {
      keyvals_[i].setValue(value);
      return;
    }

    ++i;
  }

  PSVinteger ind = start_ + used_;

  if (ind > end_) {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_DICT_FULL);

    return;
  }

  keyvals_.push_back(PSViewKeyValue());

  keyvals_[ind].setKeyValue(key, value);

  ++used_;
}

void
PSViewDictionary::
deleteValue(PSViewToken *key)
{
  int i = start_;

  int j = 0;

  for ( ; j < used_; ++j) {
    if (keyvals_[i].getKey()->compare(key) == 0)
      break;

    ++i;
  }

  if (j >= used_)
    CTHROW("Dictionary Entry Not Found");

  --used_;

  for ( ; j < used_; ++j)
    keyvals_[i].setKeyValue(keyvals_[i + 1]);

  keyvals_.pop_back();
}

void
PSViewDictionary::
clear()
{
  used_ = 0;

  keyvals_.clear();
}

//-------

PSViewKeyValue::
PSViewKeyValue(PSViewToken *key, PSViewToken *value) :
 key_(key), value_(value)
{
}

int
PSViewKeyValue::
compare(PSViewKeyValue *key_value)
{
  int cmp = key_->compare(key_value->key_);

  if (cmp != 0)
    return cmp;

  return value_->compare(key_value->value_);
}

PSViewToken *
PSViewKeyValue::
getKey()
{
  return key_;
}

PSViewToken *
PSViewKeyValue::
getValue()
{
  return value_;
}

void
PSViewKeyValue::
setKey(PSViewToken *key)
{
  key_ = key;
}

void
PSViewKeyValue::
setValue(PSViewToken *value)
{
  value_ = value;
}

void
PSViewKeyValue::
setKeyValue(PSViewToken *key, PSViewToken *value)
{
  key_   = key;
  value_ = value;
}

void
PSViewKeyValue::
setKeyValue(const PSViewKeyValue &key_value)
{
  key_   = key_value.key_;
  value_ = key_value.value_;
}
