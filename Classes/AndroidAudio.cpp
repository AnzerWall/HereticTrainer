#include "cocos2d.h"
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
#include"AndroidAudio.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;
void initAudioEngine()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "initAudioEngine", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
#endif
}
void playEffectPerfect()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "playEffectPerfect", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
#endif
}
void playEffectGreat()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "playEffectGreat", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
#endif
}
void playEffectGood()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "playEffectGood", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
#endif
}
void playEffectMiss()
{
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "playEffectMiss", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
	}
#endif
}

