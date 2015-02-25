#include "MusicPlayer.h"

 bool Prepare(const char* Filename)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "PrepareMusic", "(Ljava/lang/String;)Z"))
	{
		jstring fname = t.env->NewStringUTF(Filename);
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID, fname);
	}
    #endif
}

 bool PlayMusic()
{
	 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "PlayMusic", "()Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	}
    #endif
}
bool Seekto(int millisec)
{
	  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "SeekTo", "(I)Z"))
	{
		jint mil = (jint)millisec;
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID, mil);
	}
    #endif
}
bool PlayDuration(int start, int end)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "PlayMusic_Duration", "(II)Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID, start, end);
	}
    #endif
}
 bool Resume()
{
	 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "Continue_Play", "()Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	}
    #endif
}
 bool Stop()
{
	 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "Stop()", "()Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	}
    #endif
}
 bool Pause()
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/MusicPlay", "Pause", "()Z"))
	{
		return t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	}
    #endif
}