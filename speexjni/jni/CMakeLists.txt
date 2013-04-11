cmake_minimum_required(VERSION 2.8)

project(speex)

#notice the "recycling" of CMAKE_C_FLAGS
#this is necessary to pick up android flags
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic" )

include_directories(${PROJECT_SOURCE_DIR}/include)

add_definitions(-DFIXED_POINT -DUSE_KISS_FFT -DHAVE_CONFIG_H)

add_library(speex SHARED ./libspeex/bits.c ./libspeex/buffer.c ./libspeex/cb_search.c ./libspeex/exc_10_16_table.c ./libspeex/exc_10_32_table.c ./libspeex/exc_20_32_table.c ./libspeex/exc_5_256_table.c ./libspeex/exc_5_64_table.c ./libspeex/exc_8_128_table.c ./libspeex/fftwrap.c ./libspeex/filterbank.c ./libspeex/filters.c ./libspeex/gain_table.c ./libspeex/gain_table_lbr.c ./libspeex/hexc_10_32_table.c ./libspeex/hexc_table.c ./libspeex/high_lsp_tables.c ./libspeex/jitter.c ./libspeex/kiss_fft.c ./libspeex/kiss_fftr.c ./libspeex/lpc.c ./libspeex/lsp.c ./libspeex/lsp_tables_nb.c ./libspeex/ltp.c ./libspeex/mdf.c ./libspeex/modes.c ./libspeex/modes_wb.c ./libspeex/nb_celp.c ./libspeex/preprocess.c ./libspeex/quant_lsp.c ./libspeex/resample.c ./libspeex/sb_celp.c ./libspeex/scal.c ./libspeex/smallft.c ./libspeex/speex.c ./libspeex/speex_callbacks.c ./libspeex/speex_header.c ./libspeex/stereo.c ./libspeex/vbr.c ./libspeex/vq.c ./libspeex/window.c ./com_purplefrog_speexjni_SpeexEncoder.c ./com_purplefrog_speexjni_SpeexDecoder.c ./slots.c)

target_link_libraries(speex log m)
