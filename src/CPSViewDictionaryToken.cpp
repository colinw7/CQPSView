#include <CPSViewI.h>

PSViewDictionaryToken::
PSViewDictionaryToken(PSViewDictionaryMgr *mgr, PSVinteger max_length) :
 PSViewToken(mgr->getPSView(), PSVIEW_TOKEN_TYPE_DICTIONARY,
             PSVIEW_TOKEN_COMPOSITE,
             PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 mgr_       (mgr),
 dictionary_()
{
  dictionary_ = new PSViewDictionary(mgr, max_length);
}

PSViewDictionaryToken::
PSViewDictionaryToken(const PSViewDictionaryToken &dictionary_token) :
 PSViewToken(dictionary_token),
 mgr_       (dictionary_token.mgr_),
 dictionary_()
{
  dictionary_ = dictionary_token.dictionary_->dup();
}

PSViewDictionaryToken::
~PSViewDictionaryToken()
{
}

PSViewDictionaryToken *
PSViewDictionaryToken::
dup() const
{
  return new PSViewDictionaryToken(*this);
}

int
PSViewDictionaryToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    auto *dictionary_token = dynamic_cast<PSViewDictionaryToken *>(token);

    return dictionary_->compare(dictionary_token->dictionary_);
  }
  else
    return type_ - token->getType();
}

void
PSViewDictionaryToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for Dictionary Missing\n");
}

const PSViewName &
PSViewDictionaryToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewDictionaryToken::
print()
{
  CStrUtil::printf("-dict-");

  if (psview_->getDebug()) {
    print();

    CStrUtil::printf("<<");

    for (int i = 1; i <= getNumValues(); i++) {
      if (i > 1) CStrUtil::printf(" ");

      getKey(i)->print();

      CStrUtil::printf(" ");

      getValue(i)->print();
    }

    CStrUtil::printf(">>\n");
  }
}

std::string
PSViewDictionaryToken::
toString()
{
  return "--nostringval--";
}

int
PSViewDictionaryToken::
getNumValues()
{
  return dictionary_->getNumValues();
}

int
PSViewDictionaryToken::
getMaxValues()
{
  return dictionary_->getMaxValues();
}

void
PSViewDictionaryToken::
setValue(PSVinteger pos, PSViewToken *key, PSViewToken *value)
{
  if (getGlobal() && (key->getLocal() || value->getLocal())) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);
    return;
  }

  if (key->isString()) {
    auto *string_token = static_cast<PSViewStringToken *>(key);

    key = new PSViewNameToken(string_token);
  }

  dictionary_->setValue(pos, key, value);
}

void
PSViewDictionaryToken::
setValue(PSVinteger pos, const PSViewName &key, PSViewToken *value)
{
  if (getGlobal() && value->getLocal()) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);
    return;
  }

  auto *key1 = new PSViewNameToken(psview_, key);

  dictionary_->setValue(pos, key1, value);
}

void
PSViewDictionaryToken::
setValue(PSVinteger pos, const std::string &key, PSViewToken *value)
{
  if (getGlobal() && value->getLocal()) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);
    return;
  }

  auto *key1 = new PSViewNameToken(psview_, key);

  dictionary_->setValue(pos, key1, value);
}

void
PSViewDictionaryToken::
addValue(PSViewToken *key, PSViewToken *value)
{
  if (getGlobal() && (key->getLocal() || value->getLocal()))
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);

  if (key->isString()) {
    auto *string_token = static_cast<PSViewStringToken *>(key);

    key = new PSViewNameToken(string_token);
  }

  dictionary_->addValue(key, value);
}

void
PSViewDictionaryToken::
addValue(const PSViewName &key, PSViewToken *value)
{
  dictionary_->addValue(key, value);
}

void
PSViewDictionaryToken::
addValue(const std::string &key, PSViewToken *value)
{
  dictionary_->addValue(key, value);
}

PSViewKeyValue *
PSViewDictionaryToken::
getKeyValue(int i)
{
  return dictionary_->getKeyValue(i);
}

PSViewToken *
PSViewDictionaryToken::
getKey(int i)
{
  return dictionary_->getKey(i);
}

PSViewToken *
PSViewDictionaryToken::
getValue(int i)
{
  return dictionary_->getValue(i);
}

PSViewToken *
PSViewDictionaryToken::
getValue(PSViewToken *key)
{
  return dictionary_->getValue(key);
}

PSViewToken *
PSViewDictionaryToken::
getValue(const PSViewName &key)
{
  return dictionary_->getValue(key);
}

PSViewToken *
PSViewDictionaryToken::
getValue(const std::string &key)
{
  return dictionary_->getValue(key);
}

void
PSViewDictionaryToken::
deleteValue(PSViewToken *key)
{
  dictionary_->deleteValue(key);
}

void
PSViewDictionaryToken::
resize()
{
  dictionary_->resize();
}

void
PSViewDictionaryToken::
clear()
{
  dictionary_->clear();
}
