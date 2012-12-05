package com.purplefrog.speexjni;

import android.app.*;
import android.os.*;
import android.text.method.*;
import android.util.*;
import android.widget.*;

import java.io.*;

/**
 * Copyright 2012, Robert Forsman
 * speex-ndk@thoth.purplefrog.com
 */
public class XiphCopyrightActivity
    extends Activity
{
    public final static String LOG_TAG = "XiphCopyrightActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.xiph_copyright);

        TextView tv = (TextView) findViewById(R.id.text);
        if (null==tv) {
            Log.e(LOG_TAG, "failed to find text widget, brace for exception...");
        }
        tv.setText(getXiphCopyright());
        tv.setMovementMethod(new ScrollingMovementMethod());
    }

    public String getXiphCopyright()
    {
        InputStream istr = getResources().openRawResource(R.raw.xiph_copyright);
        Reader r = new InputStreamReader(istr);

        return slurp(r);
    }

    public static String slurp(Reader r)
    {
        StringBuilder rval = new StringBuilder();

        try {
            char[] buffer = new char[4<<10];
            while (true) {
                int n = r.read(buffer, 0, buffer.length);
                if (n<1)
                    break;
                rval.append(buffer, 0, n);
            }
        } catch (IOException e) {
            Log.e(LOG_TAG, "malfunction slurping stream", e);
        }

        return rval.toString();
    }
}
