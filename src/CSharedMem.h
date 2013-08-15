#ifndef CSHARED_MEM_H
#define CSHARED_MEM_H

template<class T>
class CSharedMemData {
 private:
  T   *memory_;
  int  size_;
  int  ref_count_;

 public:
  CSharedMemData(int size) :
   memory_(NULL), size_(size), ref_count_(1) {
    memory_ = new T [size_];
  }

 ~CSharedMemData() {
   if (ref_count_ != 0)
      CTHROW("Bad Delete");

    delete [] memory_;
  }

  const T *getData(int pos) {
    if (pos < 0 || pos > size_)
      CTHROW("Bad Subscript");

    return &memory_[pos];
  }

  const T &getValue(int pos) {
    if (pos < 0 || pos > size_)
      CTHROW("Bad Subscript");

    return memory_[pos];
  }

  void setData(int pos, const T *data, int len) {
    if (pos < 0 || pos + len - 1 > size_)
      CTHROW("Bad Subscript");

    for (int i = 0; i < len; ++i)
      memory_[pos + i] = data[i];
  }

  void setValue(int pos, const T &value) {
    if (pos < 0 || pos > size_)
      CTHROW("Bad Subscript");

    memory_[pos] = value;
  }

  void addRef() {
    ++ref_count_;
  }

  void deleteRef() {
    if (ref_count_ <= 0)
      CTHROW("Bad Ref Count");

    --ref_count_;

    if (ref_count_ == 0)
      delete this;
  }

 private:
  CSharedMemData(const CSharedMemData &rhs);
  CSharedMemData &operator=(const CSharedMemData &rhs);
};

template<class T>
class CSharedMem {
 private:
  int                start_;
  int                len_;
  CSharedMemData<T> *mem_;

 public:
  CSharedMem(int len) :
   start_(0), len_(len), mem_(NULL) {
    mem_ = new CSharedMemData<T>(len);
  }

  CSharedMem(CSharedMem *mem, int start, int len) :
   start_(mem->start_ + start), len_(len), mem_(mem->mem_) {
    if (start < 0 || start > mem->len_ ||
        len   < 0 || len   > mem->len_)
      CTHROW("Bad Subscript");

    mem_->addRef();
  }

  CSharedMem(CSharedMem *mem, int start) :
   start_(mem->start_ + start), len_(mem->len_ - start), mem_(mem->mem_) {
    if (start < 0 || start > mem->len_)
      CTHROW("Bad Subscript");

    mem_->addRef();
  }

  CSharedMem(CSharedMem *mem) :
   start_(mem->start_), len_(mem->len_), mem_(mem->mem_) {
    mem_->addRef();
  }

 ~CSharedMem() {
    mem_->deleteRef();
  }

  const T *getData() {
    return mem_->getData(start_);
  }

  const T &getValue(int pos) {
    return mem_->getValue(start_ + pos);
  }

  void setData(int pos, const T *data, int len) {
    mem_->setData(start_ + pos, data, len);
  }

  void setData(const T *data, int len) {
    mem_->setData(start_, data, len);
  }

  void setValue(int pos, const T &value) {
    mem_->setValue(start_ + pos, value);
  }

  void setBounds(int pos, int len) {
    if (pos < 0 || pos       > len_ ||
        len < 0 || pos + len > len_)
      CTHROW("Bad Indices");

    start_ = start_ + pos;
    len_   = len;
  }

  int getStart() const { return start_; }
  int getLen() const { return len_; }

 private:
  CSharedMem(const CSharedMem &rhs);
  CSharedMem &operator=(const CSharedMem &rhs);
};

#endif
