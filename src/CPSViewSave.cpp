#include "CPSViewI.h"

PSViewSave::
PSViewSave(void *data) :
 data_(data)
{
}

int
PSViewSave::
compare(PSViewSave *)
{
  return 1;
}

void *
PSViewSave::
getValue()
{
  return data_;
}
