#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include"MainGame.h"
#include "SelectSong.h"


#define TestAudio
#ifdef TestAudio
#include "audio/include/AudioEngine.h"
#else
#include "SimpleAudioEngine.h"
#endif
USING_NS_CC;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("HereticTrainer");
        director->setOpenGLView(glview);
    }
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	if (FileUtils::getInstance()->isDirectoryExist("/sdcard/heretictrainer/"))FileUtils::getInstance()->addSearchPath("/sdcard/heretictrainer/");
	if (FileUtils::getInstance()->isDirectoryExist("/mnt/storage/sdcard/heretictrainer/"))FileUtils::getInstance()->addSearchPath("/mnt/storage/sdcard/heretictrainer/");
	if (FileUtils::getInstance()->isDirectoryExist("/storage/emulated/0/heretictrainer/"))FileUtils::getInstance()->addSearchPath("/storage/emulated/0/heretictrainer/");
	if (FileUtils::getInstance()->isDirectoryExist("/storage/emulated/1/heretictrainer/"))FileUtils::getInstance()->addSearchPath("/storage/emulated/1/heretictrainer/");
	if (FileUtils::getInstance()->isDirectoryExist("/storage/extSdCard/heretictrainer/"))FileUtils::getInstance()->addSearchPath("/storage/extSdCard/heretictrainer/");
	if (FileUtils::getInstance()->isDirectoryExist("/mnt/sdcard/heretictrainer/"))FileUtils::getInstance()->addSearchPath("/mnt/sdcard/heretictrainer/");

#endif
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)
	if (FileUtils::getInstance()->isDirectoryExist("Resources/"))FileUtils::getInstance()->addSearchPath("Resources/");
	if (FileUtils::getInstance()->isDirectoryExist("../HereticTrainer/"))FileUtils::getInstance()->addSearchPath("../HereticTrainer/");

#endif
    // turn on display FPS

    director->setDisplayStats(false);
	//glview->setFrameSize(1280,720);
	glview->setDesignResolutionSize(1080, 720, ResolutionPolicy::SHOW_ALL);//·Ö±æÂÊÆ¥Åä
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
//	auto scene = MainGame::createScene(SongManager::loadjson("sghard.json"), std::string("sghard.mp3"), 1);
	//auto scene = HelloWorld::createScene();
	auto scene = SelectSong::createScene();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
#ifdef TestAudio
	experimental::AudioEngine::pauseAll();
#else
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
#endif

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
#ifdef TestAudio
	experimental::AudioEngine::resumeAll();
	
#else
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#endif 
}
