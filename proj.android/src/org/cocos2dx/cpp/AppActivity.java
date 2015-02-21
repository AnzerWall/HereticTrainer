/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

package org.cocos2dx.cpp;


import org.cocos2dx.lib.Cocos2dxActivity;


import java.lang.String;
import android.media.AudioManager;
import android.media.SoundPool;
import android.media.MediaPlayer;
import android.content.Context;
import android.os.Bundle;
import android.app.Activity;
import java.io.IOException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.os.Environment;
import android.os.StatFs;
public class AppActivity extends Cocos2dxActivity
{

    private static int idPerfect;
    private static int idGreat;
    private  static int idGood;
    private  static int  idMiss;
    private   static SoundPool sp;
    private static void initAudioEngine()
    {
        AssetManager assetManager = getContext().getAssets();
        sp=new SoundPool(20, AudioManager.STREAM_MUSIC, 0);
        try
        {

            idPerfect=sp.load(assetManager.openFd("perfect.ogg"), 1);
            idGreat=sp.load(assetManager.openFd("great.ogg"), 1);
            idGood=sp.load(assetManager.openFd("good.ogg"), 1);
            idMiss=sp.load(assetManager.openFd("miss.ogg"), 1);
        }
        catch(IOException e) {}

    }
    private static void playEffectPerfect()
    {
        sp.play(idPerfect, 1.0f, 1.0f, 1, 0, 1);
    }
    private static void playEffectGood()
    {
        sp.play(idGood, 1.0f, 1.0f, 1, 0, 1);
    }
    private static void playEffectGreat()
    {
        sp.play(idGreat, 1.0f, 1.0f, 1, 0, 1);
    }
    private static void playEffectMiss()
    {
        sp.play(idMiss, 1.0f, 1.0f, 1, 0, 1);
    }



}
