#include <stdlib.h>
#include <string.h>

#include "slots.h"

int allocate_slot(struct SlotVector * sv)
{

    if (sv->slots==0) {
	sv->nslots = 1;
	sv->slots = malloc(sizeof(struct Slot*));
	sv->slots[0] = (void*)0;
    }

    int slot;

    for (slot=0; slot<sv->nslots; slot++) {
	if ((void*)0 == sv->slots[slot])
	    break;
    }

    if (slot >= sv->nslots) {
	struct Slot** newArray = malloc( (1+sv->nslots)*sizeof(struct Slot*) );
	memcpy(newArray, sv->slots, sv->nslots * sizeof(struct Slot*));
	newArray[sv->nslots]=(void*)0;
	free(sv->slots);
	sv->slots = newArray;
	sv->nslots++;
    }

    return slot;
}
