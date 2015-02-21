#include "HelloWorldScene.h"
#include<math.h>
#include<sstream>
#include"AndroidAudio.h"
//#include "extensions/cocos-ext.h"
USING_NS_CC;
//������ҳ��
Scene* HelloWorld::createScene()
{

	
	//initAudioEngine();
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label



	// add "HelloWorld" splash screen"
	auto sprite = Sprite::create("bk_bot.png");

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite, 0);
	auto spBackGround_top = Sprite::create("bk_top.png");
	spBackGround_top->setAnchorPoint(Vec2(0, 1));
	spBackGround_top->setPosition(0, 720);
	addChild(spBackGround_top, 0);











	return true;

}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
	/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
		return;
		#endif

		Director::getInstance()->end();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
		#endif*/
//	playEffectPerfect();


}