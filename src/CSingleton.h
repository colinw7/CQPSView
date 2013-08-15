#ifndef CSINGLETON_H
#define CSINGLETON_H

/*!
  CSingleton class to use as Base class for Singleton.

  e.g.

  class MySingleton : public CSingleton<MySingleton> {
   protected:
    friend class CSingleton<MySingleton>;

    CSingleton();
  };

  The constructor should be protected to prevent use.

  Need 'friend' to allow 'new T' call to construct Dervied class.

  TODO: replace 'new T' with factory class.
*/

template<typename T>
class CSingleton {
 protected:
  CSingleton() { }

  virtual ~CSingleton() { }

  CSingleton(const CSingleton &instance);
  CSingleton &operator=(const CSingleton &instance);

 private:
  static T **getInstancePtrPtr() {
    static T *instance_;

    return &instance_;
  }

 public:
  static T *getInstancePtr() {
    T **p = getInstancePtrPtr();

    if (*p == 0)
      *p = new T;

    return *p;
  }

  static T &getInstanceRef() {
    return *getInstancePtr();
  }

  static void freeInstance() {
    T **p = getInstancePtrPtr();

    delete *p;

    *p = 0;
  }
};

#endif
