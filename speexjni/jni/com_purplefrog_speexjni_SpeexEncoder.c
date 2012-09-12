#include "com_purplefrog_speexjni_SpeexEncoder.h"

#include <stdlib.h>
#include <string.h>

#include <android/log.h>
#include "speex/speex.h"

struct Slot {
  SpeexBits	bits;
  void *enc_state;
};


static struct Slot **slots=0;
static int nslots=0;

#define LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG  , "libspeex", __VA_ARGS__) 

//
//
//

JNIEXPORT jint JNICALL Java_com_purplefrog_speexjni_SpeexEncoder_allocate
  (JNIEnv *env, jclass cls, jboolean wideband, jint quality)
{
    int slot;

    if (slots==0) {
	nslots = 1;
	slots = malloc(nslots*sizeof(struct Slot*));
	slots[0] = (void*)0;
    }

    for (slot=0; slot<nslots; slot++) {
	if ((void*)0 == slots[slot])
	    break;
    }

    if (slot >= nslots) {
	struct Slot** newArray = malloc( (1+nslots)*sizeof(struct Slot*) );
	memcpy(newArray, slots, nslots*sizeof(struct Slot*));
	newArray[nslots]=(void*)0;
	free(slots);
	slots = newArray;
	nslots++;
    }

    //

    slots[slot] = malloc(sizeof(struct Slot));

    struct Slot* gob = slots[slot];

    //

    speex_bits_init(&gob->bits);

    gob->enc_state = speex_encoder_init(wideband ? &speex_wb_mode : &speex_nb_mode);

    speex_encoder_ctl(gob->enc_state, SPEEX_SET_QUALITY, &quality);

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
    if (slot>=nslots) {
	throwIllegalArgumentException(env, "bogus slot");
	return 1;
    }

    if ((void*)0 == slots[slot]) {
	jclass newExcCls = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
	if (newExcCls == 0) /* Unable to find the new exception class, give up. */
	    return 1;
	(*env)->ThrowNew(env, newExcCls, "slot is already empty");
	return 1;
    }

    return 0; // the slot is good
}

//

JNIEXPORT void JNICALL Java_com_purplefrog_speexjni_SpeexEncoder_deallocate
  (JNIEnv *env, jclass cls, jint slot)
{
    if (throwIfBadSlot(env, slot))
	return;

    speex_bits_destroy(&slots[slot]->bits);
    speex_encoder_destroy(slots[slot]->enc_state);

    free( slots[slot] );
    slots[slot] = (void*)0;
}

//


JNIEXPORT jint JNICALL Java_com_purplefrog_speexjni_SpeexEncoder_getFrameSize
  (JNIEnv *env, jclass cls, jint slot)
{
    if (throwIfBadSlot(env, slot))
	return;

    int frame_size;
    struct Slot * gob = slots[slot];

    speex_encoder_ctl(gob->enc_state, SPEEX_GET_FRAME_SIZE, &frame_size);

    return frame_size;
}

//

JNIEXPORT jbyteArray JNICALL Java_com_purplefrog_speexjni_SpeexEncoder_encode
  (JNIEnv *env, jclass cls, jint slot, jshortArray input_frame_)
{
    if (throwIfBadSlot(env, slot))
	return;

    struct Slot * gob = slots[slot];

    int nSamples = (*env)->GetArrayLength(env, input_frame_);

    int frame_size;
    speex_encoder_ctl(gob->enc_state, SPEEX_GET_FRAME_SIZE, &frame_size);

    if (nSamples != frame_size) {
	throwIllegalArgumentException(env, "mismatch between proper frame size and supplied sample array");
	return;
    }

    //

    short* input_frame = (*env)->GetShortArrayElements(env, input_frame_, 0);
    
    speex_bits_reset(&gob->bits);

    speex_encode_int(gob->enc_state, input_frame, &gob->bits);

    (*env)->ReleaseShortArrayElements(env, input_frame_, input_frame, 0);

    //

    int nOutput = speex_bits_nbytes(&gob->bits);

    jbyteArray rval;
    rval = (*env)->NewByteArray(env, nOutput);
    if (rval==0) {
	throwOutOfMemoryError(env, "failed to allocate speex output frame");
	return;
    }

    char* output_frame = (*env)->GetByteArrayElements(env, rval, 0);

    speex_bits_write(&gob->bits, output_frame, nOutput);

    (*env)->ReleaseByteArrayElements(env, rval, output_frame, 0);

    return rval;
}
