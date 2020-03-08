#ifndef CPSViewDictionaryToken_H
#define CPSViewDictionaryToken_H

class PSViewDictionaryToken : public PSViewToken {
 public:
  PSViewDictionaryToken(PSViewDictionaryMgr *mgr, PSVinteger max_length);
  PSViewDictionaryToken(const PSViewDictionaryToken &dictionary_token);

  //----

  // virtual functions

 ~PSViewDictionaryToken();

  PSViewDictionaryToken *dup() const;

  const PSViewName &getName();

  void print();

  std::string toString();

  void executeToken();

  int compare(PSViewToken *token);

  //----

  int getNumValues();
  int getMaxValues();

  void setValue(PSVinteger pos, PSViewToken *key, PSViewToken *value);
  void setValue(PSVinteger pos, const PSViewName &key, PSViewToken *value);
  void setValue(PSVinteger pos, const std::string &key, PSViewToken *value);

  void addValue(PSViewToken *key, PSViewToken *value);
  void addValue(const PSViewName &key, PSViewToken *value);
  void addValue(const std::string &key, PSViewToken *value);

  PSViewKeyValue *getKeyValue(int i);

  PSViewToken *getKey(int i);
  PSViewToken *getValue(int i);
  PSViewToken *getValue(PSViewToken *key);
  PSViewToken *getValue(const PSViewName &key);
  PSViewToken *getValue(const std::string &key);

  void deleteValue(PSViewToken *key);

  void resize();

  void clear();

 private:
  PSViewDictionaryToken &operator=(const PSViewDictionaryToken &dictionary_token);

 private:
  PSViewDictionaryMgr        *mgr_ { nullptr };
  CAutoPtr<PSViewDictionary>  dictionary_;
};

#endif
