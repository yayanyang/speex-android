package com.purplefrog.speexjni;

/**
 * Copyright 2012, Robert Forsman
 * speex-ndk@thoth.purplefrog.com
 */
public class SpeexEncoder
{
    private final int slot;

    public SpeexEncoder(boolean wideband, int quality)
    {
        slot = allocate(wideband, quality);
    }

    @Override
    protected void finalize()
        throws Throwable
    {
        deallocate(slot);
    }

    public synchronized int getFrameSize()
    {
        return getFrameSize(slot);
    }

    public synchronized byte[] encode(short[] samples)
    {
        return encode(slot, samples);
    }

    private native static byte[] encode(int slot, short[] samples);
    private native static int getFrameSize(int slot);

    /**
     * allocates a slot in the JNI implementation for our native bits.  Store it in the {@link #slot} field.
     * @param quality from 0 to 10 inclusive, used by the speex library
     * @param wideband true for wideband, false for narrowband
     * @return an index into a slot array in the JNI implementation for our encoder parameters.
     */
    protected native static int allocate(boolean wideband, int quality);

    /**
     * @param slot the return value from a previous call to {@link #allocate(boolean, int)}
     */
    protected native static void deallocate(int slot);

    static {
        System.loadLibrary("speex");
    }

    public static void main(String[] argv)
    {
        short[] bogus = new short[666];

        byte[] frame = new SpeexEncoder(false, 9).encode(bogus);

        System.out.println(frame.length);
    }
}