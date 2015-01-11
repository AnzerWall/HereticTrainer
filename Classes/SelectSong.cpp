#include "SelectSong.h"
#include<math.h>
#include<sstream>
#include"MainGame.h"
#include "ui/CocosGUI.h"
#include"DataManager.h"
USING_NS_CC;

Scene* SelectSong::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SelectSong::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
std::string getFileName(std::string path)//得到去除后缀后的文件名
{
	std::string ret = "";
	std::string::size_type pos = std::string::npos;
	if ((pos = path.find(".json")) == (path.length() - 5))
	{
		ret = path.substr(0, path.length() - 5) ;
	}
	else if ((pos = path.find(".slist")) == (path.length() - 6))
	{
		ret = path.substr(0, path.length() - 6) ;
	}
	return ret;

}
// on "init" you need to initialize your instance
bool SelectSong::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto sprite = Sprite::create("bk2.png");
	sprite->setPosition(Vec2(540,360));
	this->addChild(sprite, 0);

	auto spPanel = Sprite::create("bk2_panel.png");
	spPanel->setPosition(Vec2(540,360));
	this->addChild(spPanel, 1);
	spPanel->setOpacity(200);

	auto btClose = ui::Button::create("CloseNormal.png");


	btClose->setPosition(Vec2(1080 - btClose->getContentSize().width/2-5,
		btClose->getContentSize().height/2+5 ));
	btClose->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->end();
		}
	});
	this->addChild(btClose, 10);



	filelist = SongManager::getFileList();
	filelist.push_back("bokuranolive.slist");

	createSprite(filelist.size() - 1);
	auto btLeft = ui::Button::create("left.png");
	btLeft->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED&&curPos > 0)
		{ 
			this->removeSprite();
			this->createSprite(curPos - 1); 
		}
	});
	btLeft->setPosition(Vec2(1080 - 850, 720 - 339));
	auto btRight= ui::Button::create("right.png");
	btRight->addTouchEventListener(
		[=](Ref *pSender, ui::Widget::TouchEventType type)
		{
		if (type == ui::Widget::TouchEventType::ENDED&&curPos <filelist.size()-1)
		{ 
			this->removeSprite();
			this->createSprite(curPos + 1);
		}
	});
	btRight->setPosition(Vec2(850, 720 - 339));
	
	addChild(btLeft,10);
	addChild(btRight,10);




	auto btBegin = ui::Button::create("btbegin.png");
	btBegin->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (filelist[curPos].size() > 0 && filelist[curPos][filelist[curPos].size() - 1] == 'n')
			{
				auto scene = MainGame::createScene(SongManager::loadjson(filelist[curPos].c_str()), FileUtils::getInstance()->fullPathForFilename(std::string(getFileName(filelist[curPos]) + ".mp3")), 1);
				Director::getInstance()->replaceScene(scene);
			}
			else
			{
				auto scene = MainGame::createScene(SongManager::load(filelist[curPos].c_str()), FileUtils::getInstance()->fullPathForFilename(std::string(getFileName(filelist[curPos]) + ".mp3")), 1);
				Director::getInstance()->replaceScene(scene);
			}
			
		}
	});
	btBegin->setPosition(Vec2(540,720- 677));
	btBegin->setAnchorPoint(Vec2(0.5,0.5));
	addChild(btBegin,13);
	return true;

}
std::string getname(std::string path)//根据文件名获取显示名称
{
	auto pos = path.find_last_of("/");
	if (pos != std::string::npos)
	{
		return path.substr(pos + 1);
	}
	else if ((pos = path.find_last_of("\\")) != std::string::npos)
	{
		return path.substr(pos + 1);
	}
	return path;
}
void SelectSong::createSprite(int pos)
{
	curPos = pos;
	
	std::string pathSprite = getFileName(filelist[pos]) + ".png";
	

	if (!FileUtils::getInstance()->isFileExist(pathSprite))pathSprite = "bokuranolive.png";
	lbName = Label::create(getname(filelist[pos]), "Arial", 42);
	lbName->setPosition(Vec2(540, 720 - 541));
	lbName->setOpacity(0);
	lbName->runAction(FadeIn::create(0.5));
	spTitle = Sprite::create(pathSprite);;
	spTitle->setOpacity(0);
	spTitle->setPosition(Vec2(540, 720 - 321));
	spTitle->runAction(FadeIn::create(0.5));
	spTitle->setContentSize(Size(300, 300));
	addChild(lbName,5);
	addChild(spTitle, 5);
}
void SelectSong::removeSprite()
{
	auto curSprite = this->spTitle;
	auto curLabel = this->lbName;
	
	curSprite->runAction(Sequence::create(FadeOut::create(0.5), CCCallFunc::create([=](){curSprite->removeFromParent(); }), NULL));
	curLabel->runAction(Sequence::create(FadeOut::create(0.5), CCCallFunc::create([=](){curLabel->removeFromParent(); }), NULL));
}