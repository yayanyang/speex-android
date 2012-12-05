#include "com_purplefrog_speexjni_SpeexDecoder.h"

#include <stdlib.h>
#include <string.h>

#include "slots.h"

#ifdef SUPPRESS_ANDROID_LOG
#define LOGD(...) do { } while(0)
#else
#include <android/log.h>

#define LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG  , "libspeex", __VA_ARGS__) 
#endif


static struct SlotVector slots = {
    0,0
};

//
//
//

JNIEXPORT jint JNICALL Java_com_purplefrog_speexjni_SpeexDecoder_allocate
  (JNIEnv *env, jclass cls, jint wideband)
{
    int slot = allocate_slot(&slots);

    //

    slots.slots[slot] = malloc(sizeof(struct Slot));

    struct Slot* gob = slots.slots[slot];

    //

    speex_bits_init(&gob->bits);


    const SpeexMode * mode;
    switch (wideband) {
    case 1:
	mode = &speex_wb_mode;
	break;
    case 2:
	mode = &speex_uwb_mode;
	break;
    default:
	mode = &speex_nb_mode;
	break;
    }

    gob->state = speex_decoder_init(mode);

    return slot;
}

//

static void throwIllegalArgumentException(JNIEnv *env, char * msg)
{
    jclass newExcCls = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
    if (newExcCls == 0) /* Unable to find the new exception class, give up. */
	return;
    (*env)->ThrowNew(env, newExcCls, msg);
}

static void throwOutOfMemoryError(JNIEnv *env, char * msg)
{
    jclass newExcCls = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
    if (newExcCls == 0) /* Unable to find the new exception class, give up. */
	return;
    (*env)->ThrowNew(env, newExcCls, msg);
}

//

static int throwIfBadSlot(JNIEnv *env, jint slot)
{
    if (slot>=slots.nslots) {
	throwIllegalArgumentException(env, "bogus slot");
	return 1;
    }

    if ((void*)0 == slots.slots[slot]) {
	jclass newExcCls = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
	if (newExcCls == 0) /* Unable to find the new exception class, give up. */
	    return 1;
	(*env)->ThrowNew(env, newExcCls, "slot is already empty");
	return 1;
    }

    return 0; // the slot is good
}

//

JNIEXPORT void JNICALL Java_com_purplefrog_speexjni_SpeexDecoder_deallocate
  (JNIEnv *env, jclass cls, jint slot)
{
    if (throwIfBadSlot(env, slot))
	return;

    speex_bits_destroy(&slots.slots[slot]->bits);
    speex_decoder_destroy(slots.slots[slot]->state);

    free( slots.slots[slot] );
    slots.slots[slot] = (void*)0;
}

//

//

JNIEXPORT jshortArray JNICALL Java_com_purplefrog_speexjni_SpeexDecoder_decode
  (JNIEnv *env, jclass cls, jint slot, jbyteArray input_frame_)
{
    if (throwIfBadSlot(env, slot))
	return;

    struct Slot * gob = slots.slots[slot];

    int frame_length = (*env)->GetArrayLength(env, input_frame_);

    int frame_size;
    speex_decoder_ctl(gob->state, SPEEX_GET_FRAME_SIZE, &frame_size);

    //

    char* input_frame = (*env)->GetByteArrayElements(env, input_frame_, 0);
    
    speex_bits_read_from(&gob->bits, input_frame, frame_length);

    (*env)->ReleaseByteArrayElements(env, input_frame_, input_frame, 0);

    //

    jshortArray rval;
    rval = (*env)->NewShortArray(env, frame_size);
    if (rval==0) {
	throwOutOfMemoryError(env, "failed to allocate speex output frame");
	return;
    }

    short* output_frame = (*env)->GetShortArrayElements(env, rval, 0);

    speex_decode_int(gob->state, &gob->bits, output_frame);

    (*env)->ReleaseShortArrayElements(env, rval, output_frame, 0);

    return rval;
}
