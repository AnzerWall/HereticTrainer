LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
			../../Classes/CustomDrawNode.cpp \
			../../Classes/DataManager.cpp \
			../../Classes/SelectSong.cpp \
			../../Classes/AndroidAudio.cpp \
			../../Classes/BJson.cpp \
			../../Classes/md5.cpp \
			../../Classes/RandomSong.cpp \
			../../Classes/SettingScene.cpp \
			../../Classes/LLParser.cpp \
			../../Classes/MainGame.cpp 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
