#ifndef PSVIEW_SHARED_MEM_H
#define PSVIEW_SHARED_MEM_H

template<class T>
class PSViewSharedMemData;

template<class T>
class PSViewSharedMem {
 private:
  int                     start_;
  int                     len_;
  PSViewSharedMemData<T> *mem_;

 public:
  PSViewSharedMem(int len);
  PSViewSharedMem(PSViewSharedMem *mem, int start, int len);
  PSViewSharedMem(PSViewSharedMem *mem, int start);
 ~PSViewSharedMem();

  const T *getData ();
  const T &getValue(int pos);

  void setData (int pos, const T *data, int len);
  void setData (const T *data, int len);
  void setValue(int pos, const T &value);

  void setBounds(int pos, int len);

  int getStart() const { return start_; }
  int getLen() const { return len_; }
};

#endif
