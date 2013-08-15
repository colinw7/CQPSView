#ifndef PSVIEW_MEMORY_I_H
#define PSVIEW_MEMORY_I_H

enum PSViewMemoryType {
  PSVIEW_MEMORY_TYPE_NONE,
  PSVIEW_MEMORY_TYPE_GLOBAL,
  PSVIEW_MEMORY_TYPE_LOCAL
};

struct PSViewSaveData;
struct PSViewSaveMemory;

struct PSViewMemory {
  ulong            id;
  ulong            size;
  PSViewMemoryType type;
  ulong            ref;
  ulong            depth;
  ulong            dummy;
};

class PSViewMemoryMgr {
 private:
  CPSView          *psview_;
  PSViewMemoryType  memory_type_;
  int               memory_depth_;
  uint              memory_used_;
  PSViewSaveData   *save_data_;

  std::list<PSViewMemory *> global_memory_list_;
  std::list<PSViewMemory *> local_memory_list_;

 public:
  PSViewMemoryMgr(CPSView *psview);
 ~PSViewMemoryMgr();

  CPSView *getPSView() const { return psview_; }

  void init();

  void getStatus(int *depth, int *used, int *max);

  PSViewToken *save();
  void         restore(PSViewToken *token);

  char *alloc(uint size);

  char *allocGlobal(uint size);
  char *allocLocal(uint size);

  void change(char **address);

  void free(char *memory);

  void free(PSViewMemory *memory);
  void free(PSViewSaveMemory *save_memory);

  void setGlobal(bool flag);

  int getDepth(char *address);

  bool getGlobal(char *address);
  bool getLocal(char *address);

  bool getGlobal();
  bool getLocal();
  int  getDepth();

 private:
  PSViewMemoryMgr(const PSViewMemoryMgr &rhs);
  PSViewMemoryMgr &operator=(const PSViewMemoryMgr &rhs);
};

#endif
