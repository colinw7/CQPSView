#ifndef PSVIEW_DICTIONARY_H
#define PSVIEW_DICTIONARY_H

class PSViewDictionaryToken;

class PSViewDictionaryMgr {
 private:
  typedef std::vector<PSViewDictionaryToken *> DictionaryStack;

  static const char *standard_encoding_[256];
  static const char *error_names_[];

  CPSView                         *psview_;
  CAutoPtr<PSViewDictionaryToken>  error_dictionary_;
  CAutoPtr<PSViewDictionaryToken>  sub_error_dictionary_;
  CAutoPtr<PSViewDictionaryToken>  internal_dictionary_;
  DictionaryStack                  dictionary_stack_;

 public:
  PSViewDictionaryMgr(CPSView *psview);
 ~PSViewDictionaryMgr();

  CPSView *getPSView() const { return psview_; }

  void init();

  int  getNumDictionaries();
  void addToCurrent(PSViewToken *key, PSViewToken *value);

  PSViewToken *lookup(const std::string &name);
  PSViewToken *lookup(const PSViewName &name);
  PSViewToken *lookup(PSViewToken *key);

  PSViewDictionaryToken *lookupDictionary(const std::string &name);
  PSViewDictionaryToken *lookupDictionary(const PSViewName &name);
  PSViewDictionaryToken *lookupDictionary(PSViewToken *key);

  PSViewDictionaryToken *getSystemDictionary();
  PSViewDictionaryToken *getGlobalDictionary();
  PSViewDictionaryToken *getUserDictionary();
  PSViewDictionaryToken *getInternalDictionary();
  PSViewDictionaryToken *getCurrentDictionary();
  PSViewDictionaryToken *getDictionary(int num);

  void beginDictionary(PSViewDictionaryToken *dictionary);
  void endDictionary();

  void emptyDictionaryStack();

 private:
  PSViewDictionaryMgr(const PSViewDictionaryMgr &rhs);
  PSViewDictionaryMgr &operator=(const PSViewDictionaryMgr &rhs);

 private:
  void                   createError();
  PSViewDictionaryToken *createSystem();
  PSViewDictionaryToken *createGlobal();
  PSViewDictionaryToken *createUser();
};

class PSViewKeyValue {
 private:
  PSViewToken *key_;
  PSViewToken *value_;

 public:
  explicit
  PSViewKeyValue(PSViewToken *key = NULL, PSViewToken *value = NULL);

  int compare(PSViewKeyValue *key_value);

  PSViewToken *getKey();
  PSViewToken *getValue();

  void setKey  (PSViewToken *key);
  void setValue(PSViewToken *value);

  void setKeyValue(PSViewToken *key, PSViewToken *value);
  void setKeyValue(const PSViewKeyValue &key_value);
};

class PSViewDictionary {
 private:
  PSViewDictionaryMgr         *mgr_;
  std::vector<PSViewKeyValue>  keyvals_;
  PSVinteger                   start_;
  PSVinteger                   end_;
  PSVinteger                   used_;

 public:
  PSViewDictionary(PSViewDictionaryMgr *mgr, PSVinteger max_length);
  PSViewDictionary(const PSViewDictionary &dictionary);
 ~PSViewDictionary();

  PSViewDictionary *dup() const;

  CPSView *getPSView() const { return mgr_->getPSView(); }

  int compare(PSViewDictionary *dictionary);

  int getNumValues();
  int getMaxValues();

  void addEntry(PSViewToken *key, PSViewToken *value);

  PSViewKeyValue *getKeyValue(int i) const;

  PSViewToken *getKey(int i);
  PSViewToken *getValue(int i);
  PSViewToken *getValue(PSViewToken *key);
  PSViewToken *getValue(const PSViewName &name);
  PSViewToken *getValue(const std::string &str);

  void setValue(PSVinteger pos, PSViewToken *key, PSViewToken *value);

  void resize();

  void addValue(const std::string &name, PSViewToken *value);
  void addValue(const PSViewName &key, PSViewToken *value);
  void addValue(PSViewToken *key, PSViewToken *value);

  void deleteValue(PSViewToken *key);

  void clear();

 private:
  PSViewDictionary &operator=(const PSViewDictionary &dictionary);
};

#endif
