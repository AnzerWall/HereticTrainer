#include "SettingScene.h"


USING_NS_CC;


Scene* SettingScene::scene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	SettingScene *layer = SettingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
void SettingScene::setSilder()
{
	slGood->setPercent(UserDefault::getInstance()->getDoubleForKey("gooddis") / 144 * 100);
	lbGood->setString(String::createWithFormat("%d", (int)UserDefault::getInstance()->getDoubleForKey("gooddis"))->getCString());

	slGreat->setPercent(UserDefault::getInstance()->getDoubleForKey("greatdis") / 144 * 100);
	lbGreat->setString(String::createWithFormat("%d", (int)UserDefault::getInstance()->getDoubleForKey("greatdis"))->getCString());

	slPrefect->setPercent(UserDefault::getInstance()->getDoubleForKey("perfectdis") / 144 * 100);
	lbPerfect->setString(String::createWithFormat("%d", (int)UserDefault::getInstance()->getDoubleForKey("perfectdis"))->getCString());

	slBad->setPercent(UserDefault::getInstance()->getDoubleForKey("baddis") / 144 * 100);
	lbBad->setString(String::createWithFormat("%d", (int)UserDefault::getInstance()->getDoubleForKey("baddis"))->getCString());
}
bool SettingScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	auto sprite = Sprite::create("view_bk_start.png");
	sprite->setPosition(Vec2(540, 360));
	this->addChild(sprite, 0);


	slPrefect = ui::Slider::create();
	slPrefect->loadBarTexture("slider_track.png");
	slPrefect->loadSlidBallTextures("slider_thumb.png", "slider_thumb.png", "");
	slPrefect->loadProgressBarTexture("slider_progress.png");
	slPrefect->setPosition(Vec2(609,720-168));
	slPrefect->addEventListener(CC_CALLBACK_2(SettingScene::sliderEventPerfect, this));
	addChild(slPrefect,13);

	slGreat = ui::Slider::create();
	slGreat->loadBarTexture("slider_track.png");
	slGreat->loadSlidBallTextures("slider_thumb.png", "slider_thumb.png", "");
	slGreat->loadProgressBarTexture("slider_progress.png");
	slGreat->setPosition(Vec2(609, 720 - 276));
	slGreat->addEventListener(CC_CALLBACK_2(SettingScene::sliderEventGreat, this));
	addChild(slGreat, 13);

	slGood = ui::Slider::create();
	slGood->loadBarTexture("slider_track.png");
	slGood->loadSlidBallTextures("slider_thumb.png", "slider_thumb.png", "");
	slGood->loadProgressBarTexture("slider_progress.png");
	slGood->setPosition(Vec2(609, 720 - 364));
	slGood->addEventListener(CC_CALLBACK_2(SettingScene::sliderEventGood, this));

	addChild(slGood, 13);


	slBad= ui::Slider::create();
	slBad->loadBarTexture("slider_track.png");
	slBad->loadSlidBallTextures("slider_thumb.png", "slider_thumb.png", "");
	slBad->loadProgressBarTexture("slider_progress.png");
	slBad->setPosition(Vec2(609, 720 - 451));
	slBad->addEventListener(CC_CALLBACK_2(SettingScene::sliderEventBad, this));
	addChild(slBad, 13);

	auto lbPFC = Label::create("Perfect标准:", "Arial", 24);
	lbPFC->setPosition(Vec2(206, 720 - 168));
	addChild(lbPFC, 13);
	auto lbGRC = Label::create("Great标准:", "Arial", 24);
	lbGRC->setPosition(Vec2(206, 720 - 276));
	addChild(lbGRC, 13);
	auto lbGDC = Label::create("Good标准:", "Arial", 24);
	lbGDC->setPosition(Vec2(206, 720 - 364));
	addChild(lbGDC, 13);
	auto lbBDC = Label::create("Bad标准:", "Arial", 24);
	lbBDC->setPosition(Vec2(206, 720 - 451));
	addChild(lbBDC, 13);

	lbPerfect = Label::create("100", "Arial",18);
	lbPerfect->setPosition(Vec2(935, 720 - 168));
	addChild(lbPerfect, 13);
	lbGreat = Label::create("100", "Arial", 18);
	lbGreat->setPosition(Vec2(935, 720 - 276));
	addChild(lbGreat, 13);
	lbGood = Label::create("100", "Arial", 18);
	lbGood->setPosition(Vec2(935, 720 - 364));
	addChild(lbGood, 13);
	lbBad = Label::create("100", "Arial", 18);
	lbBad->setPosition(Vec2(935, 720 - 451));
	addChild(lbBad, 13);


	auto btReturn = ui::Button::create("button_return.png");
	btReturn->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			UserDefault::getInstance()->flush();
			Director::getInstance()->popScene();
			

		}
	});
	btReturn->setPosition(Vec2(51, 720 - 42));
	btReturn->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(btReturn, 13);

	auto btReset = ui::Button::create("button_reset.png");
	btReset->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			
			UserDefault::getInstance()->setDoubleForKey("baddis", 144);
			UserDefault::getInstance()->setDoubleForKey("gooddis", 80);
			UserDefault::getInstance()->setDoubleForKey("greatdis", 50);
			UserDefault::getInstance()->setDoubleForKey("perfectdis", 20);
			setSilder();
		}
	});
	btReset->setPosition(Vec2(540, 720 - 626));
	btReset->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(btReset, 13);
	/*UserDefault::getInstance()->setDoubleForKey("rate", 1.0);
	UserDefault::getInstance()->setDoubleForKey("baddis", 144);
	UserDefault::getInstance()->setDoubleForKey("gooddis", 120);
	UserDefault::getInstance()->setDoubleForKey("greatdis", 80);
	UserDefault::getInstance()->setDoubleForKey("perfectdis", 45);
	UserDefault::getInstance()->setDoubleForKey("touchdis", 142);
	UserDefault::getInstance()->setDoubleForKey("touchwidth", 80);
	UserDefault::getInstance()->setDoubleForKey("touchheight", 140);*/
	setSilder();
	return true;
}
void SettingScene::sliderEventPerfect(Ref *pSender, cocos2d::ui::Slider::EventType type)
{
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		double val = ( slPrefect->getPercent()/100.0) * 144;
		if (val > UserDefault::getInstance()->getDoubleForKey("greatdis") || val<5)
		{
			slPrefect->setPercent(UserDefault::getInstance()->getDoubleForKey("perfectdis") / 144 * 100);
		}
		else
		{
			UserDefault::getInstance()->setDoubleForKey("perfectdis", val);
		}
		val = (slPrefect->getPercent() / 100.0) * 144;
		lbPerfect->setString(String::createWithFormat("%d", (int)val)->getCString());
	}
}
void SettingScene::sliderEventGreat(Ref *pSender, cocos2d::ui::Slider::EventType type)
{
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		double val = (slGreat->getPercent() / 100.0) * 144;
		if (val > UserDefault::getInstance()->getDoubleForKey("gooddis") || val<UserDefault::getInstance()->getDoubleForKey("perfectdis"))
		{
			slGreat->setPercent(UserDefault::getInstance()->getDoubleForKey("greatdis") / 144 * 100);
		}
		else
		{
			UserDefault::getInstance()->setDoubleForKey("greatdis", val);
		}
		val = (slGreat->getPercent() / 100.0) * 144;
		lbGreat->setString(String::createWithFormat("%d", (int)val)->getCString());
	}
}
void SettingScene::sliderEventGood(Ref *pSender, cocos2d::ui::Slider::EventType type)
{
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		double val = (slGood->getPercent() / 100.0) * 144;
		if (val > UserDefault::getInstance()->getDoubleForKey("baddis") || val<UserDefault::getInstance()->getDoubleForKey("greatdis"))
		{
			slGood->setPercent(UserDefault::getInstance()->getDoubleForKey("gooddis") / 144 * 100);
		}
		else
		{
			UserDefault::getInstance()->setDoubleForKey("gooddis", val);
		}
		val = (slGood->getPercent() / 100.0) * 144;
		lbGood->setString(String::createWithFormat("%d", (int)val)->getCString());
	}
}
void SettingScene::sliderEventBad(Ref *pSender, cocos2d::ui::Slider::EventType type)
{
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		double val = (slBad->getPercent() / 100.0) * 144;

		if (val > 144 || val<UserDefault::getInstance()->getDoubleForKey("gooddis"))
		{
			slBad->setPercent(UserDefault::getInstance()->getDoubleForKey("baddis") / 144 * 100);
		}
		else
		{
			UserDefault::getInstance()->setDoubleForKey("baddis", val);
		}
		val = (slBad->getPercent() / 100.0) * 144;
		lbBad->setString(String::createWithFormat("%d", (int)val)->getCString());
	}
}