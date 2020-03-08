#ifndef CPSViewSave_H
#define CPSViewSave_H

class PSViewSave {
 public:
  PSViewSave(void *data);

  int compare(PSViewSave *save);

  void *getValue();

 private:
  void *data_ { nullptr };
};

#endif
