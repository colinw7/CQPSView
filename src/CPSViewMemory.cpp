#include <CPSViewI.h>

const uint MEMORY_ID = 0xDEAFDEAD;

struct PSViewSaveMemory {
  PSViewMemory  *new_memory;
  PSViewMemory  *old_memory;
  char         **address;

  PSViewSaveMemory() :
   new_memory(nullptr),
   old_memory(nullptr),
   address   (nullptr) {
  }
};

struct PSViewSaveData {
  int                           depth_;
  std::list<PSViewSaveMemory *> save_memory_list_;

  PSViewSaveData() :
   depth_           (0),
   save_memory_list_() {
  }
};

/*---- Data ----*/

PSViewMemoryMgr::
PSViewMemoryMgr(CPSView *psview) :
 psview_            (psview),
 memory_type_       (PSVIEW_MEMORY_TYPE_LOCAL),
 memory_depth_      (1),
 memory_used_       (0),
 save_data_         (nullptr),
 global_memory_list_(),
 local_memory_list_ ()
{
  init();
}

PSViewMemoryMgr::
~PSViewMemoryMgr()
{
}

void
PSViewMemoryMgr::
init()
{
  std::list<PSViewMemory *>::iterator pmemory;

  for (pmemory = global_memory_list_.begin();
        pmemory != global_memory_list_.end(); ++pmemory)
    delete *pmemory;

  for (pmemory = local_memory_list_.begin();
        pmemory != local_memory_list_.end(); ++pmemory)
    delete *pmemory;

  global_memory_list_.clear();
  local_memory_list_ .clear();

  /*----*/

  memory_type_  = PSVIEW_MEMORY_TYPE_LOCAL;
  memory_depth_ = 1;
  memory_used_  = 0;

  /*----*/

  save();
}

void
PSViewMemoryMgr::
getStatus(int *depth, int *used, int *max)
{
  *depth = memory_depth_;
  *used  = int(memory_used_);
  *max   = -1;
}

PSViewToken *
PSViewMemoryMgr::
save()
{
  memory_depth_++;

  save_data_ = new PSViewSaveData;

  save_data_->depth_ = memory_depth_;

  auto *token = new PSViewSaveToken(getPSView(), save_data_);

  return token;
}

void
PSViewMemoryMgr::
restore(PSViewToken *token)
{
  auto *save_token = static_cast<PSViewSaveToken *>(token);

  save_data_ = static_cast<PSViewSaveData *>(save_token->getValue());

  std::list<PSViewSaveMemory *>::iterator psave_memory;

  for (psave_memory = save_data_->save_memory_list_.begin();
        psave_memory != save_data_->save_memory_list_.end(); ++psave_memory)
    delete *psave_memory;

  memory_depth_ = save_data_->depth_ - 1;

  int num = getPSView()->getOperandStack()->size();

  for (int i = num; i >= 1; i--) {
    PSViewToken *token1 = getPSView()->getOperandStack()->peek(i);

    if (token1->getMemoryDepth() > memory_depth_) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_RESTORE);

      return;
    }
  }

  delete save_data_;
}

char *
PSViewMemoryMgr::
alloc(uint size)
{
  char *memory;

  if (getGlobal())
    memory = allocGlobal(size);
  else
    memory = allocLocal(size);

  return memory;
}

char *
PSViewMemoryMgr::
allocGlobal(uint size)
{
  auto *memory = reinterpret_cast<PSViewMemory *>(new char [sizeof(PSViewMemory) + size]);

  if (memory == nullptr) {
    CStrUtil::eprintf("PSView: Fatal Memory Error\n");
    exit(1);
  }

  memory->id    = MEMORY_ID;
  memory->size  = size;
  memory->type  = PSVIEW_MEMORY_TYPE_GLOBAL;
  memory->ref   = 1;
  memory->depth = ulong(memory_depth_);

  global_memory_list_.push_back(memory);

  memory_used_ += size;

  return (reinterpret_cast<char *>(memory) + sizeof(PSViewMemory));
}

char *
PSViewMemoryMgr::
allocLocal(uint size)
{
  auto *memory = reinterpret_cast<PSViewMemory *>(new char [sizeof(PSViewMemory) + size]);

  if (memory == nullptr) {
    CStrUtil::eprintf("PSView: Fatal Memory Error\n");
    exit(1);
  }

  memory->id    = MEMORY_ID;
  memory->size  = size;
  memory->type  = PSVIEW_MEMORY_TYPE_LOCAL;
  memory->ref   = 1;
  memory->depth = uint(memory_depth_);

  local_memory_list_.push_back(memory);

  memory_used_ += size;

  return (reinterpret_cast<char *>(memory) + sizeof(PSViewMemory));
}

void
PSViewMemoryMgr::
change(char **address)
{
  auto *memory = reinterpret_cast<PSViewMemory *>(*address - sizeof(PSViewMemory));

  if (memory->id != MEMORY_ID) {
    CStrUtil::eprintf("PSView: Fatal Memory Error\n");
    exit(1);
  }

  if (memory->type == PSVIEW_MEMORY_TYPE_GLOBAL || int(memory->depth) >= memory_depth_)
    return;

  auto *save_memory = new PSViewSaveMemory;

  if (save_memory == nullptr) {
    CStrUtil::eprintf("PSView: Fatal Memory Error\n");
    exit(1);
  }

  char *address1 = allocLocal(uint(memory->size));

  memcpy(address1, *address, memory->size);

  save_memory->new_memory = reinterpret_cast<PSViewMemory *>(address1 - sizeof(PSViewMemory));
  save_memory->old_memory = memory;
  save_memory->address    = address;

  save_data_->save_memory_list_.push_back(save_memory);

  *address = address1;
}

void
PSViewMemoryMgr::
free(char *)
{
  return;
}

void
PSViewMemoryMgr::
free(PSViewMemory *memory)
{
  delete memory;
}

void
PSViewMemoryMgr::
free(PSViewSaveMemory *save_memory)
{
  delete [] save_memory->new_memory;

  *save_memory->address =
    (reinterpret_cast<char *>(save_memory->old_memory) + sizeof(PSViewMemory));

  delete save_memory;
}

void
PSViewMemoryMgr::
setGlobal(bool flag)
{
  if (flag)
    memory_type_ = PSVIEW_MEMORY_TYPE_GLOBAL;
  else
    memory_type_ = PSVIEW_MEMORY_TYPE_LOCAL;
}

int
PSViewMemoryMgr::
getDepth(char *address)
{
  auto *memory = reinterpret_cast<PSViewMemory *>(address - sizeof(PSViewMemory));

  if (memory->id != MEMORY_ID) {
    CStrUtil::eprintf("PSView: Fatal Memory Error\n");
    exit(1);
  }

  return int(memory->depth);
}

bool
PSViewMemoryMgr::
getGlobal(char *address)
{
  auto *memory = reinterpret_cast<PSViewMemory *>(address - sizeof(PSViewMemory));

  if (memory->id != MEMORY_ID) {
    CStrUtil::eprintf("PSView: Fatal Memory Error\n");
    exit(1);
  }

  return (memory->type == PSVIEW_MEMORY_TYPE_GLOBAL);
}

bool
PSViewMemoryMgr::
getLocal(char *address)
{
  auto *memory = reinterpret_cast<PSViewMemory *>(address - sizeof(PSViewMemory));

  if (memory->id != MEMORY_ID) {
    CStrUtil::eprintf("PSView: Fatal Memory Error\n");
    exit(1);
  }

  return (memory->type == PSVIEW_MEMORY_TYPE_LOCAL);
}

bool
PSViewMemoryMgr::
getGlobal()
{
  return (memory_type_ == PSVIEW_MEMORY_TYPE_GLOBAL);
}

bool
PSViewMemoryMgr::
getLocal()
{
  return (memory_type_ == PSVIEW_MEMORY_TYPE_LOCAL);
}

int
PSViewMemoryMgr::
getDepth()
{
  return memory_depth_;
}
