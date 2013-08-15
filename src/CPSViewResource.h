#ifndef PSVIEW_RESOURCE_H
#define PSVIEW_RESOURCE_H

class PSViewResource;

class PSViewResourceMgr {
 private:
  typedef std::vector<PSViewResource *> ResourceList;

  CPSView      *psview_;
  ResourceList  resources_;

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
};

class PSViewResource {
 private:
  PSViewResourceMgr *mgr_;
  PSViewToken       *key_;
  PSViewToken       *instance_;
  const PSViewName  &category_;

 public:
  PSViewResource(PSViewResourceMgr *mgr, PSViewToken *key,
                 PSViewToken *instance, const PSViewName &category);

  PSViewToken      *getKey     () const { return key_     ; }
  PSViewToken      *getInstance() const { return instance_; }
  const PSViewName &getCategory() const { return category_; }
};


#endif
