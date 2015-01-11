#include "MainGame.h"
#include "SelectSong.h"

#include <algorithm>
#include<sstream>
#include<math.h>
#define TestAudio
#ifdef TestAudio
#include "audio/include/AudioEngine.h"
#else
#include "SimpleAudioEngine.h"
#endif

USING_NS_CC;
using namespace ui;
Vec2 vGameArea[] = { Vec2(89, 539), Vec2(124, 364), Vec2(220, 219), Vec2(367, 122), Vec2(539, 88)
, Vec2(712, 122), Vec2(858, 219), Vec2(954, 364), Vec2(989, 539) };

Vec2 vBornPoint = Vec2(539, 539);
double R = 62;

std::string musicpath;

Scene* MainGame::createScene(const Song &song, std::string songpath, double speedrate)
{
	// 'scene' is an autorelease object

	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainGame::create();

	// add layer as a child to scene
	scene->addChild(layer);
	layer->song = song;
	layer->rate = speedrate;
	layer->songpath = songpath;
	// return the scene
	return scene;
}
void MainGame::showPressEffect(int pos)
{
	auto spRing = Sprite::create("ring.png");
	spRing->setPosition(vGameArea[pos]);
	spRing->setScale(0.4);
	auto sqRing = Sequence::create(Spawn::create(FadeOut::create(0.2), ScaleTo::create(0.1, 1), NULL),
		CCCallFuncN::create([=](Ref *sender){this->removeChild(spRing); }), NULL);;
	spRing->runAction(sqRing);
	this->addChild(spRing, 5);

	auto spStar = Sprite::create("star.png");
	spStar->setPosition(vGameArea[pos]);
	auto sqStar = Sequence::create(DelayTime::create(0.1), FadeOut::create(0.1),
		CCCallFuncN::create([=](Ref *sender){this->removeChild(spStar); }), NULL);
	spStar->runAction(sqStar);
	this->addChild(spStar, 5);


}
// on "init" you need to initialize your instance
bool MainGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
//	idAudio = experimental::AudioEngine::INVAILD_AUDIO_ID;
	
	//创建背景
	auto spBackGround = Sprite::create("bk_bot.png");
	//spBackGround->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	spBackGround->setPosition(540,360);
	addChild(spBackGround, 0);
	auto spBackGround_top = Sprite::create("bk_top.png");
	spBackGround_top->setAnchorPoint(Vec2(0,1));
	spBackGround_top->setPosition(0, 720);
	addChild(spBackGround_top, 0);

	//创建头像
	for (int i = 0; i < 9; i++)
	{
		auto sptmp = Sprite::create("umi.png");
		sptmp->setPosition(vGameArea[i]);
		addChild(sptmp, 5);
	}
	//创建触摸层
	touchLayer = Layer::create();
	touchLayer->setAnchorPoint(Vec2::ZERO);
	touchLayer->setPosition(Vec2::ZERO);
	touchLayer->setContentSize(Size(1080,720));
	//touchLayer->setOpacity(0);
	addChild(touchLayer, 20);

	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(MainGame::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(MainGame::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(MainGame::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, touchLayer);

	

	//评分精灵创建
	spPerfect = Sprite::create("perfect.png");
	spGreat = Sprite::create("great.png");
	spGood = Sprite::create("good.png");
	spBad = Sprite::create("bad.png");
	spMiss = Sprite::create("miss.png");
	spPerfect->setVisible(false);
	spGreat->setVisible(false);
	spGood->setVisible(false);
	spBad->setVisible(false);
	spMiss->setVisible(false);
	spPerfect->setPosition(Vec2(540, 360));
	spGreat->setPosition(Vec2(540, 360));
	spGood->setPosition(Vec2(540, 360));
	spBad->setPosition(Vec2(540, 360));
	spMiss->setPosition(Vec2(540, 360));

	addChild(spPerfect,30);
	addChild(spGreat, 30);
	addChild(spGood, 30);
	addChild(spBad, 30);
	addChild(spMiss, 30);
	//showScoreEffect(Score::PREJECT);


	//圆环出生点 音符动画
	auto spRing1 = Sprite::create("ring2.png");
	auto spRing2 = Sprite::create("ring2.png");
	auto spRing3 = Sprite::create("ring2.png");
	auto spRing4 = Sprite::create("ring2.png");
	spRing1->setPosition(vBornPoint);
	spRing2->setPosition(vBornPoint);
	spRing3->setPosition(vBornPoint);
	spRing4->setPosition(vBornPoint);
	spRing1->setOpacity(0);
	spRing2->setOpacity(0);
	spRing3->setOpacity(0);
	spRing4->setOpacity(0);
	auto sqRing1 = RepeatForever::create(Sequence::create(
		CCCallFuncN::create([=](Ref *sender){((Sprite*)sender)->setScale(0.2); ((Sprite*)sender)->setOpacity(255); }),
		EaseOut::create(Spawn::create(ScaleTo::create(0.6, 1), FadeOut::create(0.6), NULL), 5),
		DelayTime::create(1.4),
		NULL));
	auto sqRing2 = RepeatForever::create(Sequence::create(
		DelayTime::create(0.2),
		CCCallFuncN::create([=](Ref *sender){((Sprite*)sender)->setScale(0.2); ((Sprite*)sender)->setOpacity(255); }),
		EaseOut::create(Spawn::create(ScaleTo::create(0.6, 1), FadeOut::create(0.6), NULL), 5),
		DelayTime::create(1.2),
		NULL));
	auto sqRing3 = RepeatForever::create(Sequence::create(
		DelayTime::create(0.4),
		CCCallFuncN::create([=](Ref *sender){((Sprite*)sender)->setScale(0.2); ((Sprite*)sender)->setOpacity(255); }),
		EaseOut::create(Spawn::create(ScaleTo::create(0.6, 1), FadeOut::create(0.6), NULL), 5),
		DelayTime::create(1.0),
		NULL));
	auto sqRing4 = RepeatForever::create(Sequence::create(
		DelayTime::create(0.6),
		CCCallFuncN::create([=](Ref *sender){((Sprite*)sender)->setScale(0.2); ((Sprite*)sender)->setOpacity(255); }),
		EaseOut::create(Spawn::create(ScaleTo::create(0.6, 1), FadeOut::create(0.6), NULL), 5),
		DelayTime::create(0.8),
		NULL));
	spRing1->runAction(sqRing1);
	spRing2->runAction(sqRing2);
	spRing3->runAction(sqRing3);
	spRing4->runAction(sqRing4);
	addChild(spRing1, 9);
	addChild(spRing2, 9);
	addChild(spRing3, 9);
	addChild(spRing4, 9);
	auto spOnfuku = Sprite::create("onfuku.png");
	spOnfuku->setPosition(vBornPoint);
	spOnfuku->setScale(0.8);
	auto sqOnfuku = RepeatForever::create(Sequence::create(EaseIn::create(ScaleTo::create(1, 1), 2), EaseIn::create(ScaleTo::create(1, 0.8), 2), NULL));
	spOnfuku->runAction(sqOnfuku);
	addChild(spOnfuku, 10);


	//暂停按钮

	btStop = ui::Button::create("stopbutton.png");
	btStop->setPosition(Vec2(1050, 692));
	btStop->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
			this->StopSence(); 
	});
	addChild(btStop, 0);


	//combo显示
	lbCombo = Label::create("combo", "Arial", 30);
	lbCombo->setPosition(Vec2(570,415));
	lbCombo->setVisible(false);
	addChild(lbCombo);
	lbComboCnt = Label::create("141", "Arial", 55);
	lbComboCnt->setPosition(Vec2(490, 415));
	lbComboCnt->setAlignment(TextHAlignment::RIGHT);
	addChild(lbComboCnt);
	lbComboCnt->setVisible(false);
	

	//开启循环制造旋律
	this->scheduleUpdate();
	//播放音乐
	

	return true;
}



void  MainGame::StopSence()
{
//	TextureCache::getInstance()->removeUnusedTextures();
#ifdef TestAudio
	experimental::AudioEngine::pauseAll();
#else
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
#endif



	RenderTexture *renderTexture = RenderTexture::create(1080, 720);
	renderTexture->begin();
	//renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	this->getParent()->visit();
	renderTexture->end();
	Director::getInstance()->getRenderer()->render();
	auto scStop = Scene::create();
	//TextureCache::getInstance()->removeUnusedTextures();

	auto lyStop = Layer::create();
	scStop->addChild(lyStop);

	auto spBackGround = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	spBackGround->setPosition(540, 360);
	spBackGround->setFlippedY(true);
	//spBackGround->setScale(0.5);
	lyStop->addChild(spBackGround, 0);
	auto spStop = Sprite::create("stopscene.png");
	spStop->setPosition(540, 360);
	lyStop->addChild(spStop, 1);
	auto btContinue = ui::Button::create("button2.png");
	btContinue->setPosition(Vec2(376, 194));
	btContinue->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->popScene();
#ifdef TestAudio
		experimental::AudioEngine::resumeAll();
#else
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#endif
	} });
	lyStop->addChild(btContinue, 2);
	auto btReturn = ui::Button::create("button1.png");
	btReturn->setPosition(Vec2(704, 194));
	btReturn->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type){if (type == ui::Widget::TouchEventType::ENDED) {
		_eventDispatcher->removeEventListener(this->listener);

#ifdef TestAudio
		experimental::AudioEngine::stopAll();
#else
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
#endif
		this->endGame = true;
		Director::getInstance()->popScene(); 
		
		//Director::getInstance()->popScene();

	} });
	lyStop->addChild(btReturn, 2);
	Director::getInstance()->pushScene(scStop);

}

void MainGame::showScoreEffect(Score score)
{

	//log("ttt");
	spPerfect->stopAllActions();
	spGreat->stopAllActions();
	spGood->stopAllActions();
	spBad->stopAllActions();
	spMiss->stopAllActions();
	spPerfect->setVisible(false);
	spGreat->setVisible(false);
	spGood->setVisible(false);
	spBad->setVisible(false);
	spMiss->setVisible(false);
	std::stringstream buf;
	switch (score)
	{
	case Score::PREJECT:
		spPerfect->setVisible(true);
		spPerfect->setOpacity(255);
		spPerfect->setScale(0.4);
		spPerfect->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(true);
		lbCombo->setVisible(true);

		lbComboCnt->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.1, 1.2), 2), EaseIn::create(ScaleTo::create(0.1, 1), 2), NULL));
		lbCombo->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.1, 1.2), 2), EaseIn::create(ScaleTo::create(0.1, 1), 2), NULL));
		curCombo++;
		maxCombo = std::max(maxCombo, curCombo);
		buf << curCombo;
		lbComboCnt->setString(buf.str());
		cntPerfect++;
#ifdef TestAudio
		experimental::AudioEngine::play2d("perfect.mp3");
#else
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("perfect.mp3");
#endif
	
		break;
	case Score::GREAT:
		spGreat->setVisible(true);
		spGreat->setOpacity(255);
		spGreat->setScale(0.4);
		spGreat->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(true);
		lbCombo->setVisible(true);

		lbComboCnt->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.1, 1.2), 2), EaseIn::create(ScaleTo::create(0.1, 1), 2), NULL));
		lbCombo->runAction(Sequence::create(EaseIn::create(ScaleTo::create(0.1, 1.2), 2), EaseIn::create(ScaleTo::create(0.1, 1), 2), NULL));
		
		curCombo++;
		maxCombo = std::max(maxCombo, curCombo);
		buf << curCombo;
		lbComboCnt->setString(buf.str());
		cntGreat++;
#ifdef TestAudio
		experimental::AudioEngine::play2d("great.mp3");
#else
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("great.mp3");
#endif

		break;
	case Score::GOOD:
		spGood->setVisible(true);
		spGood->setOpacity(255);
		spGood->setScale(0.4);
		spGood->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));

		lbComboCnt->setVisible(false);
		lbCombo->setVisible(false);
		curCombo = 0;
		cntGood++;
#ifdef TestAudio
		experimental::AudioEngine::play2d("good.mp3");
#else
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("good.mp3");
#endif

		break;
	case Score::BAD:
		spBad->setVisible(true);
		spBad->setOpacity(255);
		spBad->setScale(0.4);
		spBad->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(false);
		lbCombo->setVisible(false);
		curCombo = 0;
		cntBad++;
#ifdef TestAudio
		experimental::AudioEngine::play2d("good.mp3");
#else
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("good.mp3");
#endif

		break;
	case Score::MISS:
		spMiss->setVisible(true);
		spMiss->setOpacity(255);
		spMiss->setScale(0.4);
		spMiss->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(false);
		lbCombo->setVisible(false);
#ifdef TestAudio
		experimental::AudioEngine::play2d("good.mp3");
#else
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("good.mp3");
#endif

		curCombo = 0;
		cntMiss++;
		break;
	default:
		break;
	}
}
void MainGame::born(const Rhythm &rh)
{
	GameObject gobj;
	gobj.type = rh.type;
	if (rh.type&RHYTHMTYPE_LONG)//长型圆环（长旋律）
	{
		gobj.start = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME )? "r3.png" : "r2.png");

		gobj.start->setPosition(539, 539);
		gobj.start->setScale(0);//长圆环的头圆环

		gobj.goal = Sprite::create("r1.png");
		gobj.goal->setPosition(539, 539);
		gobj.goal->setScale(0);

		gobj.node = CustomDrawNode::create();//画梯形
		gobj.node->setPosition(Vec2::ZERO);
		gobj.node->setAnchorPoint(Vec2::ZERO);
		gobj.node->start = gobj.start;
		gobj.node->goal = gobj.goal;
		gobj.node->pattern = true;
		gobj.node->tm = 0;
		gobj.node->atm = rh.endTime-rh.beginTime;//持续时间


		addChild(gobj.start, 10);
		addChild(gobj.node, 8);
		addChild(gobj.goal, 10);
		q[rh.pos].push(gobj);//添加到队列，便于触摸时判断最近的圆环

		float start_t1 = this->song.speed*this->rate;//头圆环出生到到达头像处需要的时间
		float dis = vGameArea[rh.pos].distance(vBornPoint);//该道的头像和出生点的距离


		float start_t2 = (missdis / dis)*start_t1;//判定为miss需要的时间


		float end_t1 = (rh.endTime - rh.beginTime);//尾圆环动作延迟时间
		float end_t2 = this->song.speed*this->rate;//尾圆环到达头像处需要的时间


		//计算miss点，坐标
		float speedx = (vGameArea[rh.pos].x - vBornPoint.x) / (this->song.speed*this->rate);
		float speedy = (vGameArea[rh.pos].y - vBornPoint.y) / (this->song.speed*this->rate);
		float end_t3 = (missdis / dis)*end_t2;
		Vec2 vGoal = { vGameArea[rh.pos].x + speedx*end_t3, vGameArea[rh.pos].y + speedy*end_t3 };//miss时的位置
		//动作
		Sequence *sq1 = Sequence::create(
			Spawn::create(ScaleTo::create(start_t1, 1), MoveTo::create(start_t1, vGameArea[rh.pos]), NULL), DelayTime::create(start_t2)
			, CCCallFuncN::create([=](Ref*sender)
		{
			this->removeChild(gobj.start);
			this->removeChild(gobj.node);
			this->removeChild(gobj.goal);
			q[rh.pos].pop();
			showScoreEffect(Score::MISS);
		})
			, NULL);

		Sequence *sq2 = Sequence::create(
			DelayTime::create(end_t1), Spawn::create(ScaleTo::create(end_t2, 1), MoveTo::create(end_t2, vGameArea[rh.pos]), NULL), MoveTo::create(end_t3, vGoal)
			, CCCallFuncN::create([=](Ref*sender)
		{
			this->removeChild(gobj.start);
			this->removeChild(gobj.node);
			this->removeChild(gobj.goal);
			q[rh.pos].pop();

			showScoreEffect(Score::MISS);
		})
		, NULL);

		gobj.start->runAction(sq1);
		gobj.goal->runAction(sq2);
		gobj.node->scheduleUpdate();
	}
	else
	{
		gobj.start = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME) ? "r3.png" : "r2.png");
		gobj.start->setPosition(539, 539);
		gobj.start->setScale(0);
		this->addChild(gobj.start, 10);



		//这里的song.speed并不表示速度，详细见定义
		float t1 = this->song.speed*this->rate;
		float dis = vGameArea[rh.pos].distance(vBornPoint);
		float speedx = (vGameArea[rh.pos].x - vBornPoint.x)/(this->song.speed*this->rate);
		float speedy = (vGameArea[rh.pos].y - vBornPoint.y) / (this->song.speed*this->rate);
		float t2 = (missdis/dis)*t1;
		Vec2 vGoal = { vGameArea[rh.pos].x + speedx*t2, vGameArea[rh.pos].y+speedy*t2};//miss时的位置
		q[rh.pos].push(gobj);
		auto ac = Sequence::create(
			Spawn::create(ScaleTo::create(t1, 1), MoveTo::create(t1, vGameArea[rh.pos]), NULL),
			MoveTo::create(t2, vGoal),
			CCCallFuncN::create([=](Ref *sender)
		{
			
			this->removeChild(gobj.start);
			q[rh.pos].pop();
			showScoreEffect(Score::MISS);
		}), NULL);
		gobj.start->runAction(ac);
		

		
	}
}
bool MainGame::checkTouch(int pos, const Vec2 &touchLocation)//这里采取的判定区域是在圆内+长方形内
{
	float dis = vGameArea[pos].distance(vBornPoint);
	Rect checkRect = Rect(vGameArea[pos].x - touchwid, vGameArea[pos].y - touchhei, touchwid*2,touchhei*2);
	auto point=touchLocation.rotateByAngle(vGameArea[pos], atan((vBornPoint.x - vGameArea[pos].x) / (vBornPoint.y - vGameArea[pos].y)));
	return (checkRect.containsPoint(point) && touchLocation.distance(vGameArea[pos]) < touchdis);
	
}
void MainGame::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
	for (auto touch : touches)
	{
		auto location = touch->getLocation();
		for (int i = 0; i < 9;i++ )
		{
			if (checkTouch(i,location))
			{
				if (q[i].empty())break;
				auto tmp = q[i].front();
				if (tmp.type&RHYTHMTYPE_LONG)
				{
					auto sppos = tmp.start->getPosition();
					float dis = vGameArea[i].distance(sppos);
					if (dis>baddis)break;
						showPressEffect(i);
						//长条形被按住，停止动作并设置到头像处
						table.insert(std::make_pair(touch, i));
						tmp.start->stopAllActions();
						tmp.start->setPosition(vGameArea[i]);
						tmp.start->setScale(1);
						tmp.node->runAction(RepeatForever::create
							(Sequence::create(EaseIn::create(FadeOut::create(1), 2), EaseIn::create(FadeIn::create(1), 2), NULL)));
						//评价判定
						if (dis<perfectdis)
						{
							showScoreEffect(Score::PREJECT);
						}
						else if (dis < greatdis)
						{
							showScoreEffect(Score::GREAT);
						}
						else if (dis <gooddis)
						{
							showScoreEffect(Score::GOOD);
						}
						else 
						{
							showScoreEffect(Score::BAD);
						}
					
					
					
				}
				else
				{
					auto sppos=tmp.start->getPosition();

					float dis = vGameArea[i].distance(sppos);
					if (dis>baddis)break;
						showPressEffect(i);
						this->removeChild(tmp.start);
						q[i].pop();
						if (dis<perfectdis)
						{
							showScoreEffect(Score::PREJECT);
						}
						else if (dis < greatdis)
						{
							showScoreEffect(Score::GREAT);
						}
						else if (dis <gooddis)
						{
							showScoreEffect(Score::GOOD);
						}
						else
						{
							showScoreEffect(Score::BAD);
						}
					
				}
				
				break;
			}
		}
	}
	
}

void MainGame::update(float dt)
{
	if (endGame)//是否要结束游戏
	{
		Director::getInstance()->replaceScene(SelectSong::createScene());
	}
	if (!songbegin)//播放歌曲
	{
#ifdef TestAudio
		experimental::AudioEngine::play2d(songpath, false, 0.6);
#else

		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.4);
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(songpath.c_str(), false);
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.4);

#endif

		//idAudio = experimental::AudioEngine::play2d(this->songpath,false,0.6);
	
		
		songbegin = true;
	}
	curTime += dt;
	if ((song.duration<= curTime))//歌曲是否已经到达结束时间了
	{

		this->unscheduleUpdate();
#ifdef TestAudio
		experimental::AudioEngine::stopAll();
#else
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
#endif
		ResultScene();
		return;
	}

	while (curRhythm <song.data.size() && (song.data[curRhythm].beginTime - (song.speed*rate)) <= curTime)
	{
		
		born(song.data[curRhythm++]);
		
	}
	
}
void MainGame::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
	
}

void MainGame::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
	for (auto touch : touches)
	{
		auto iter = table.find(touch);
		if (iter != table.end())
		{
			auto tmp = q[iter->second].front();
			auto sppos = tmp.goal->getPosition();
			float dis = vGameArea[iter->second].distance(sppos);
			showPressEffect(iter->second);
			if (dis > missdis)
			{
				showScoreEffect(Score::MISS);
			}
			else
			{
				if (dis<perfectdis)
				{
					showScoreEffect(Score::PREJECT);
				}
				else if (dis < greatdis)
				{
					showScoreEffect(Score::GREAT);
				}
				else if (dis <gooddis)
				{
					showScoreEffect(Score::GOOD);
				}
				else
				{
					showScoreEffect(Score::BAD);
				}
			}
			
			this->removeChild(tmp.start);
			this->removeChild(tmp.node);
			this->removeChild(tmp.goal);
			q[iter->second].pop();
			table.erase(iter);

		}
	}
	
}

void MainGame::onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event)
{
	onTouchesEnded(touches, event);
}



void  MainGame::ResultScene()
{
	//屏幕截图作为背景
	RenderTexture *renderTexture = RenderTexture::create(1080, 720);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();
	Director::getInstance()->getRenderer()->render();

	//创建结果场景
	auto scResult = Scene::create();


	auto lyResult = Layer::create();
	scResult->addChild(lyResult);

	auto spBackGround = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	spBackGround->setPosition(540, 360);
	spBackGround->setFlippedY(true);
	lyResult->addChild(spBackGround, 0);


	auto spResult = Sprite::create("result.png");
	spResult->setPosition(540, 360);
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);


	listener->onTouchBegan = [=](Touch* touch, Event* event)
	{

		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
		//	_eventDispatcher->removeEventListener(listener);
		
		//	_eventDispatcher->removeAllEventListeners();

			Director::getInstance()->replaceScene(SelectSong::createScene());
			return true;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spResult);
	lyResult->addChild(spResult, 1);


	TTFConfig ttfConfig("fonts/arial.ttf", 42, GlyphCollection::DYNAMIC, nullptr, false);
	auto lbPerfect = Label::createWithTTF(ttfConfig, String::createWithFormat("%d",cntPerfect)->_string);
	lbPerfect->setColor(Color3B(255, 102, 153));
	lbPerfect->setPosition(Vec2(141, 720 - 586));
	lbPerfect->setAdditionalKerning(20);
	lyResult->addChild(lbPerfect, 2);

	auto lbGreat = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", cntGreat)->_string);
	lbGreat->setColor(Color3B(255, 102, 153));
	lbGreat->setPosition(Vec2(346, 720 - 586));
	lbGreat->setAdditionalKerning(20);
	lyResult->addChild(lbGreat, 2);

	auto lbGood = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", cntGood)->_string);
	lbGood->setColor(Color3B(255, 102, 153));
	lbGood->setPosition(Vec2(546, 720 - 586));
	lbGood->setAdditionalKerning(20);
	lyResult->addChild(lbGood, 2);

	auto lbBad = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", cntBad)->_string);
	lbBad->setColor(Color3B(255, 102, 153));
	lbBad->setPosition(Vec2(737, 720 - 586));
	lbBad->setAdditionalKerning(20);
	lyResult->addChild(lbBad, 2);

	auto lbMiss = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", cntMiss)->_string);
	lbMiss->setColor(Color3B(255, 102, 153));
	lbMiss->setPosition(Vec2(940, 720 - 586));
	lbMiss->setAdditionalKerning(20);
	lyResult->addChild(lbMiss, 2);



	auto lbScore = Label::createWithTTF(ttfConfig, "100000000");
	lbScore->setColor(Color3B(255, 102, 153));
	lbScore->setPosition(Vec2(655, 720 - 332));
	lbScore->setAdditionalKerning(20);
	lyResult->addChild(lbScore, 2);

	auto lbHScore = Label::createWithTTF(ttfConfig, "100000000");
	lbHScore->setColor(Color3B(255, 102, 153));
	lbHScore->setPosition(Vec2(655, 720 - 376));
	lbHScore->setAdditionalKerning(20);
	lyResult->addChild(lbHScore, 2);

	auto lbCombo = Label::createWithTTF(ttfConfig, String::createWithFormat("%d", maxCombo)->_string);
	lbCombo->setColor(Color3B(255, 102, 153));
	lbCombo->setPosition(Vec2(655, 720 - 458));
	lbCombo->setAdditionalKerning(20);
	lyResult->addChild(lbCombo, 2);



	Director::getInstance()->replaceScene(scResult);
	
}

