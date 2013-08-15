class PSViewSave {
 private:
  void *data_;

 public:
  PSViewSave(void *data);

  int compare(PSViewSave *save);

  void *getValue();
};
