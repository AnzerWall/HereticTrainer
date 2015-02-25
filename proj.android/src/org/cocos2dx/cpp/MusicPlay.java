package org.cocos2dx.cpp;

import java.io.IOException;

import android.media.MediaPlayer;
import android.os.Environment;
import android.util.Log;

public class MusicPlay {
	//所有方法均返回Boolean以表示是否执行成功
	 private static MediaPlayer mp = new MediaPlayer();
	 private static class timer extends Thread
	 {
		 int e;
		 public timer(int e)
		 {
			 this.e = e;
		 }
		 @Override
			public void run()
			{
			 try
			 {
				mp.start();
				while(mp.getCurrentPosition() < e && mp.isPlaying());	//如果播放被暂停则线程结束
				mp.pause();
			 }
			 catch(IllegalStateException e)
			 {
				 e.printStackTrace();
			 }
			}
	 }
	 public static String getMyStorage()
	 {
		return Environment.getExternalStorageDirectory() + "/HereticTrainer/";
	 }
	 public static boolean Continue_Play()
	 {
		 try
		 {
			Log.d("HT_mAudioEngine","Trying to Continue Playing");
			mp.start();
			return true;
		 }
		 catch(IllegalStateException e)
		 {
	    	Log.d("HT_mAudioEngine","Failed to Continue");
			 return false;
		 }
	 }
	 public static boolean PlayMusic_Duration(int start, int end)
	 {
		 //播放音乐文件，从start到end
		 try
		 {
			if(start < end && end < mp.getDuration())
			{
				Log.d("HT_mAudioEngine","Trying to PlayMusic from " + start + " to " + end + " ms");
				mp.seekTo(start);
				new MusicPlay.timer(end).start();
				return true;
			}
			else
			{
	    		Log.d("HT_mAudioEngine","Failed to Play");
				return false;
			}
		 }
		 catch(IllegalStateException e)
		 {
	    		Log.d("HT_mAudioEngine","Failed to Play");
			 return false;
		 }
		 
	 }
	 public static boolean PrepareMusic(String Filename)
	 {
	   	//Filename不包含扩展名，假设为全名
    	mp.reset();
		Log.d("HT_mAudioEngine","Trying to Load Music File");
    	String[] exts = new String[]{".mp3",".ogg",".aac",".m4a",".wav"};
    	int i = 0;
    	for(; i < exts.length;i++)
    	{
    		String FullPath = getMyStorage() + Filename + exts[i];
    		Log.d("Path","Trying to find music at " + FullPath);
    		try {
    				mp.setDataSource(FullPath);
    				mp.prepare();
				}
	   			catch (Exception e) {
	   				continue;
	   			}
	   			break;
    	}
    	if(i < 5)
	    	return true;
    	else
    	{
    		Log.d("HT_mAudioEngine","Failed to Load");
    		return false;
    	}
     }
	    public static boolean Pause()
	    {
	    	if(mp.isPlaying())
	    	{
	    		try{
	    			Log.d("HT_mAudioEngine","Trying to Pause");
		    		
	    		mp.pause();
	    		}
	    		catch(IllegalStateException e)
	    		{
		    		Log.d("HT_mAudioEngine","Failed to Pause");
		    		return false;
	    		}
	    	}
	    	return true;
	    }
	    public static boolean PlayMusic()
	    {
	    	try
	    	{
	    		Log.d("HT_mAudioEngine","Trying to PlayMusic");
	    		mp.start();
	    		return true;
	    	}
	    	catch(IllegalStateException e)
	    	{
	    		Log.d("HT_mAudioEngine","Failed to Play");
	    		return false;
	    	}
	    }
	    public static boolean SeekTo(int millisec)
	    {
	    	try
	    	{
	    		if(millisec < mp.getDuration())
	    		{
	   			 	Log.d("HT_mAudioEngine","Trying to Seek to " + millisec);
	    			mp.seekTo(millisec);
	    			return true;
	    		}
	    		else
	    		{
	    			Log.d("HT_mAudioEngine","Failed to seek");
	    			return false;
	    		}
	    		
	    	}
	    	catch(IllegalStateException e)
	    	{
	    		Log.d("HT_mAudioEngine","Failed to seek");
	    		return false;
	    	}
	    	
	    }
	 public static boolean Stop()
	 {
		 try
		 {
			 mp.stop();
			 Log.d("HT_mAudioEngine","Trying to Stop");
			 return true;
		 }
		 catch(IllegalStateException e)
		 {
			 Log.d("HT_mAudioEngine","Failed to Stop");
			 return false;
		 }
	 }
}
