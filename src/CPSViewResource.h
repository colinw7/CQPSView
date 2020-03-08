#ifndef CPSViewResource_H
#define CPSViewResource_H

class PSViewResource;

class PSViewResourceMgr {
 public:
  typedef std::vector<PSViewResource *> ResourceList;

 public:
  PSViewResourceMgr(CPSView *psview);
 ~PSViewResourceMgr();

  CPSView *getPSView() const { return psview_; }

  void init();

  PSViewToken *findInstance(PSViewToken *key, const PSViewName &category);

  ResourceList getResources(const PSViewName &category);

  bool checkInstance(PSViewToken *instance, const PSViewName &category);

  PSViewToken *lookupInstance(PSViewToken *key, const PSViewName &category);

  void define(PSViewToken *key, PSViewToken *instance, const PSViewName &category);

 private:
  PSViewResourceMgr(const PSViewResourceMgr &rhs);
  PSViewResourceMgr &operator=(const PSViewResourceMgr &rhs);

 private:
  CPSView      *psview_;
  ResourceList  resources_;
};

//---

class PSViewResource {
 public:
  PSViewResource(PSViewResourceMgr *mgr, PSViewToken *key,
                 PSViewToken *instance, const PSViewName &category);

  PSViewToken      *getKey     () const { return key_     ; }
  PSViewToken      *getInstance() const { return instance_; }
  const PSViewName &getCategory() const { return category_; }

 private:
  PSViewResourceMgr *mgr_;
  PSViewToken       *key_;
  PSViewToken       *instance_;
  const PSViewName  &category_;
};

#endif
