
JDK_HOME=/opt/sun-jdk-1.6.0.31

SPEEX_HOME=/home/thoth/vendor/speex-1.2rc1

SRCS1=jni/com_purplefrog_speexjni_SpeexDecoder.c \
	jni/com_purplefrog_speexjni_SpeexEncoder.c \
	jni/slots.c

SRCS2=	jni/libspeex/bits.c \
	jni/libspeex/buffer.c \
	jni/libspeex/cb_search.c \
	jni/libspeex/exc_10_16_table.c \
	jni/libspeex/exc_10_32_table.c \
	jni/libspeex/exc_20_32_table.c \
	jni/libspeex/exc_5_256_table.c \
	jni/libspeex/exc_5_64_table.c \
	jni/libspeex/exc_8_128_table.c \
	jni/libspeex/fftwrap.c \
	jni/libspeex/filterbank.c \
	jni/libspeex/filters.c \
	jni/libspeex/gain_table.c \
	jni/libspeex/gain_table_lbr.c \
	jni/libspeex/hexc_10_32_table.c \
	jni/libspeex/hexc_table.c \
	jni/libspeex/high_lsp_tables.c \
	jni/libspeex/jitter.c \
	jni/libspeex/kiss_fft.c \
	jni/libspeex/kiss_fftr.c \
	jni/libspeex/lpc.c \
	jni/libspeex/lsp.c \
	jni/libspeex/lsp_tables_nb.c \
	jni/libspeex/ltp.c \
	jni/libspeex/mdf.c \
	jni/libspeex/modes.c \
	jni/libspeex/modes_wb.c \
	jni/libspeex/nb_celp.c \
	jni/libspeex/preprocess.c \
	jni/libspeex/quant_lsp.c \
	jni/libspeex/resample.c \
	jni/libspeex/sb_celp.c \
	jni/libspeex/scal.c \
	jni/libspeex/smallft.c \
	jni/libspeex/speex.c \
	jni/libspeex/speex_callbacks.c \
	jni/libspeex/speex_header.c \
	jni/libspeex/stereo.c \
	jni/libspeex/vbr.c \
	jni/libspeex/vq.c \
	jni/libspeex/window.c
SRCS=$(SRCS1) $(SRCS2)


JSRC= src/com/purplefrog/speexjni/SpeexDecoder.java src/com/purplefrog/speexjni/SpeexEncoder.java src/com/purplefrog/speexjni/FrequencyBand.java

#
#
#

all: 	libspeex.so libspeex.jar

#SPEEX_FLAGS=-DFIXED_POINT -DUSE_KISS_FFT -DEXPORT=""
SPEEX_FLAGS=-DFLOATING_POINT -DUSE_SMALLFT -DEXPORT=""

SPEEX_LIB=$(SPEEX_HOME)/libspeex/.libs/libspeex.a

libspeex.so: $(SRCS)
	$(CC) -I$(JDK_HOME)/include/linux -I$(JDK_HOME)/include/ \
	$(SPEEX_FLAGS) -DSUPPRESS_ANDROID_LOG \
	-o $@ -shared $(SRCS) -static


libspeex2.so: $(SRCS1)
	$(CC) -I$(JDK_HOME)/include/linux -I$(JDK_HOME)/include/ \
	-I$(SPEEX_HOME)/include -DSUPPRESS_ANDROID_LOG \
	-o $@ -shared $(SRCS1) $(SPEEX_LIB) -static



libspeex.jar: $(JSRC)
	mkdir -p target/classes
	javac $(JSRC) -d target/classes
	jar cf $@ target/classes


frustration: frustration.c
	$(CC) -g -std=c99 -I$(SPEEX_HOME)/include $<  $(SPEEX_LIB) -lm -o $@
