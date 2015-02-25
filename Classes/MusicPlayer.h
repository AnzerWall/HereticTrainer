#ifndef MUSICPLAYER_H_INCLUDED
#define MUSICPLAYER_H_INCLUDED
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "cocos2d.h"
#include "jni.h"
#include "platform/android/jni/JniHelper.h"
#endif
bool Prepare(const char* Filename);
bool Pause();
bool PlayMusic();
bool PlayDuration(int start, int end);
bool Seekto(int millisec);
bool Resume();
bool Stop();
USING_NS_CC;

#endif // MUSICPLAYER_H_INCLUDED
