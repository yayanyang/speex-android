#include "speex/speex.h"

struct Slot {
  SpeexBits	bits;
  void *state;
};

struct SlotVector
{
  struct Slot **slots;
  int nslots;
};

int allocate_slot(struct SlotVector*slots_ptr);
