#include <CPSViewI.h>

PSViewResourceMgr::
PSViewResourceMgr(CPSView *psview) :
 psview_   (psview),
 resources_()
{
}

PSViewResourceMgr::
~PSViewResourceMgr()
{
}

void
PSViewResourceMgr::
init()
{
}

PSViewToken *
PSViewResourceMgr::
findInstance(PSViewToken *key, const PSViewName &category)
{
  if (category.compare(getPSView()->getNameMgr()->getName("Font")) == 0) {
    PSViewGStateToken *gstate_token =
      getPSView()->getGStateTokenMgr()->getCurrent();

    PSViewToken *instance = gstate_token->findFont(key);

    if (instance == NULL) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESOURCE);

      return NULL;
    }

    define(key, instance, category);

    return instance;
  }
  else {
    getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

    return NULL;
  }
}

std::vector<PSViewResource *>
PSViewResourceMgr::
getResources(const PSViewName &category)
{
  std::vector<PSViewResource *> resources;

  auto num = resources_.size();

  for (uint i = 1; i <= num; i++)
    if (resources_[i]->getCategory().compare(category) == 0)
      resources.push_back(resources_[i]);

  return resources;
}

bool
PSViewResourceMgr::
checkInstance(PSViewToken *instance, const PSViewName &category)
{
  if (category.compare(getPSView()->getNameMgr()->getName("Font")) == 0) {
    if (! instance->isDictionary()) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return false;
    }
  }

  return true;
}

PSViewToken *
PSViewResourceMgr::
lookupInstance(PSViewToken *key, const PSViewName &category)
{
  auto num = resources_.size();

  for (uint i = 0; i < num; i++) {
    if (resources_[i]->getCategory(). compare(category) == 0 &&
        resources_[i]->getKey     ()->compare(key     ) == 0)
      return resources_[i]->getInstance();
  }

  return NULL;
}

void
PSViewResourceMgr::
define(PSViewToken *key, PSViewToken *instance, const PSViewName &category)
{
  auto *resource = new PSViewResource(this, key, instance, category);

  resources_.push_back(resource);
}

//-------

PSViewResource::
PSViewResource(PSViewResourceMgr *mgr, PSViewToken *key,
               PSViewToken *instance, const PSViewName &category) :
 mgr_(mgr), key_(key), instance_(instance), category_(category)
{
}
