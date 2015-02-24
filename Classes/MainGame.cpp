#include "MainGame.h"
#include "SelectSong.h"

#include <algorithm>
#include<sstream>
#include<math.h>


#include"audio\include\AudioEngine.h"
#include"AndroidAudio.h"

USING_NS_CC;
using namespace ui;
Vec2 vGameArea[] = { Vec2(89, 539), Vec2(124, 364), Vec2(220, 219), Vec2(367, 122), Vec2(539, 88)
, Vec2(712, 122), Vec2(858, 219), Vec2(954, 364), Vec2(989, 539) };

Vec2 vBornPoint = Vec2(539, 539);
double iRadius = 62;



Scene* MainGame::createScene(const SongInfo &songinfo, const Song &song, const SongConfig &songfig)
{
	// 'scene' is an autorelease object

	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainGame::create(songinfo, song, songfig);

	// add layer as a child to scene
	scene->addChild(layer);
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
bool MainGame::init(const SongInfo &songinfo, const Song &song, const SongConfig &songfig)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	log("Anzer init begin");
	//播放音乐
	//log("Anzer init end");
	if (songconfig.bPlayMusic)
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if(Prepare(songinfo.sMusicPath.c_str()))
		{
			PlayMusic();
		}
#else
		experimental::AudioEngine::play2d(songinfo.sMusicPath);
#endif
	//log("Anzer init end2");
	memset(queueHead, 0, sizeof(queueHead));
	this->song = song;
	this->songinfo = songinfo;
	this->songconfig = songfig;
	Init_Background();
	Init_Spr_Score_cb();
	Init_TouchLayer();
	//暂停按钮
	btStop = ui::Button::create("stopbutton.png");
	btStop->setPosition(Vec2(1050, 692));
	btStop->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{
		if (type == ui::Widget::TouchEventType::ENDED)
			this->StopSence(); 
	});
	addChild(btStop, 0);
	//开启循环制造旋律
	this->scheduleUpdate();
	return true;
}
void MainGame::Init_Background()
{
	//创建背景
	auto spBackGround = Sprite::create(songinfo.sBackgroundPath,Rect(0,0,1080,720));
	spBackGround->setPosition(540,360);
	addChild(spBackGround, 0);
	//创建头像
	for (int i = 0; i < 9; i++)
	{
		auto sptmp = Sprite::create(songinfo.sPressButtonPath[i]);
		sptmp->setPosition(vGameArea[i]);
		addChild(sptmp, 5);
	}
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
	//波动的圈圈创建完毕
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
		auto spOnfuku = Sprite::create("onfuku.png");	//音乐标志，圈圈从这里出
		spOnfuku->setPosition(vBornPoint);
		spOnfuku->setScale(0.8);
		auto sqOnfuku = RepeatForever::create(Sequence::create(EaseIn::create(ScaleTo::create(1, 1), 2), EaseIn::create(ScaleTo::create(1, 0.8), 2), NULL));
		spOnfuku->runAction(sqOnfuku);
		addChild(spOnfuku, 10);
		//波动的圈圈动作设定完毕，播放动画
}
void MainGame::Init_Spr_Score_cb()
{
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

	
		//combo显示
	lbCombo = Label::create("combo", "Arial", 30);
	lbCombo->setPosition(Vec2(570,415));
	lbCombo->setVisible(false);
	addChild(lbCombo);
	lbComboCnt = Label::create("141", "Arial", 55);
	lbComboCnt->setPosition(Vec2(490, 415));
	lbComboCnt->setAlignment(TextHAlignment::RIGHT);
	addChild(lbComboCnt);
	lbComboCnt->setVisible(false);;
}
void MainGame::Init_TouchLayer()
{
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
}
void  MainGame::StopSence()
{
//	TextureCache::getInstance()->removeUnusedTextures();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Pause();
#else
		experimental::AudioEngine::pauseAll(); 
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
	//按钮继续的事件函数
	btContinue->setPosition(Vec2(376, 194));
	btContinue->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type)
	{if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->popScene();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Resume();
#else
		experimental::AudioEngine::resumeAll(); 
#endif
	} });
	//
	lyStop->addChild(btContinue, 2);
	//放弃按钮的事件函数
	auto btReturn = ui::Button::create("button1.png");
	btReturn->setPosition(Vec2(704, 194));
	btReturn->addTouchEventListener([=](Ref *pSender, ui::Widget::TouchEventType type){if (type == ui::Widget::TouchEventType::ENDED) {
		_eventDispatcher->removeEventListener(this->listener);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Stop();
#else
		experimental::AudioEngine::stopAll(); 
#endif
		Director::getInstance()->popScene(); 
		
		Director::getInstance()->popScene();

	} });
	//
	lyStop->addChild(btReturn, 2);
	Director::getInstance()->pushScene(scStop);

}

void MainGame::showScoreEffect(Score score)
{
//	static  int cnt = 0;
//	log("Anzer showScore-%d begin", cnt);
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
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectPerfect();
#else
		experimental::AudioEngine::play2d("perfect.ogg");
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
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectGreat();
#else
		experimental::AudioEngine::play2d("great.ogg");
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
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectGood();
#else
		experimental::AudioEngine::play2d("good.ogg");
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
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectGood();
#else
		experimental::AudioEngine::play2d("good.ogg");
#endif

		break;
	case Score::MISS:
		spMiss->setVisible(true);
		spMiss->setOpacity(255);
		spMiss->setScale(0.4);
		spMiss->runAction(Sequence::create(Show::create(), ScaleTo::create(0.1, 1), FadeOut::create(0.3), NULL));
		lbComboCnt->setVisible(false);
		lbCombo->setVisible(false);
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
		playEffectMiss();
#else
		experimental::AudioEngine::play2d("miss.ogg");
#endif

		curCombo = 0;
		cntMiss++;
		break;
	default:
		break;
	}
//	log("Anzer  showScore-%d end", cnt++);
}
void MainGame::born(const Rhythm &rh)
{
	//static int debug_cnt = 0;
//	log("Anzer  born-%d begin", debug_cnt);
	NodeInfo nodeinfo;

	nodeinfo.type = rh.type;
	if (rh.type&RHYTHMTYPE_LONG)//长型圆环（长旋律）
	{
		nodeinfo.head = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME) ? "r3.png" : "r2.png");
		nodeinfo.head->setPosition(539, 539);
		nodeinfo.head->setScale(0);//长圆环的头圆环

		nodeinfo.tail = Sprite::create("r1.png");
		nodeinfo.tail->setPosition(539, 539);
		nodeinfo.tail->setScale(0);

		nodeinfo.noodle = CustomDrawNode::create();//画梯形
		nodeinfo.noodle->setPosition(Vec2::ZERO);
		nodeinfo.noodle->setAnchorPoint(Vec2::ZERO);
		nodeinfo.noodle->head = nodeinfo.head;
		nodeinfo.noodle->tail = nodeinfo.tail;
		nodeinfo.noodle->pattern = true;
		nodeinfo.noodle->tm = 0;
		nodeinfo.noodle->atm = rh.endTime - rh.beginTime;//持续时间


		
		addChild(nodeinfo.head, 10);
		addChild(nodeinfo.noodle, 8);
		addChild(nodeinfo.tail, 10);

		nodeQueue[rh.pos].push_back(nodeinfo);
		nodeinfo.index = nodeQueue[rh.pos].size() - 1;
		double speed = this->song.dSpeed;
		float start_t1 = speed*this->songconfig.rate;//头圆环出生到到达头像处需要的时间
		float dis = vGameArea[rh.pos].distance(vBornPoint);//该道的头像和出生点的距离

		float start_t2 = (this-> songconfig.baddis/ dis)*start_t1;//判定为miss需要的时间

		float end_t1 = (rh.endTime - rh.beginTime)*this->songconfig.rate;//尾圆环动作延迟时间
		float end_t2 = speed*this->songconfig.rate;//尾圆环到达头像处需要的时间

		//计算miss点，坐标
		float speedx = (vGameArea[rh.pos].x - vBornPoint.x) / (speed*this->songconfig.rate);
		float speedy = (vGameArea[rh.pos].y - vBornPoint.y) / (speed*this->songconfig.rate);
		float end_t3 = (this->songconfig.baddis / dis)*end_t2;
		Vec2 vGoal =Vec2 ( vGameArea[rh.pos].x + speedx*end_t3, vGameArea[rh.pos].y + speedy*end_t3 );//miss时的位置
		//动作
		Sequence *sq1 = Sequence::create(
		Spawn::create(ScaleTo::create(start_t1, 1), MoveTo::create(start_t1, vGameArea[rh.pos]), NULL),					DelayTime::create(start_t2)
		, CCCallFuncN::create([=](Ref*sender)
		{
			nodeinfo.head->setVisible(false);
			nodeinfo.tail->setVisible(false);
			nodeinfo.noodle->setVisible(false);


		
			Score result = nodeQueue[rh.pos][nodeinfo.index].result;

			if (result == Score::NONE)
				nodeQueue[rh.pos][nodeinfo.index].result = Score::MISS;

			if (result == Score::NONE)
			showScoreEffect(Score::MISS);
			
		})
			, NULL);

		Sequence *sq2 = Sequence::create(
			DelayTime::create(end_t1), Spawn::create(ScaleTo::create(end_t2, 1), MoveTo::create(end_t2, vGameArea[rh.pos]), NULL), MoveTo::create(end_t3, vGoal)
			, CCCallFuncN::create([=](Ref*sender)
		{
			nodeinfo.head->setVisible(false);
			nodeinfo.tail->setVisible(false);
			nodeinfo.noodle->setVisible(false);
			
			Score result = nodeQueue[rh.pos][nodeinfo.index].result_tail;

			if (result == Score::NONE)
				nodeQueue[rh.pos][nodeinfo.index].result_tail = Score::MISS;

			if (result == Score::NONE)
				showScoreEffect(Score::MISS);
		})
			, DelayTime::create(10), CCCallFuncN::create([=](Ref*sender)
		{
			this->removeChild(nodeinfo.head);
			this->removeChild(nodeinfo.tail);
			this->removeChild(nodeinfo.noodle);
		})

		, NULL);

		nodeinfo.head->runAction(sq1);
		nodeinfo.tail->runAction(sq2);
		nodeinfo.noodle->scheduleUpdate();
	}
	else
	{
		nodeinfo.head = Sprite::create((rh.type&RHYTHMTYPE_SAMETIME) ? "r3.png" : "r2.png");
		nodeinfo.head->setPosition(539, 539);
		nodeinfo.head->setScale(0);
		this->addChild(nodeinfo.head, 10);



		//这里的song.speed并不表示速度，详细见定义
		double speed =this->song.dSpeed;

		float t1 = speed*this->songconfig.rate;
		float dis = vGameArea[rh.pos].distance(vBornPoint);
		float speedx = (vGameArea[rh.pos].x - vBornPoint.x) / (speed*this->songconfig.rate);
		float speedy = (vGameArea[rh.pos].y - vBornPoint.y) / (speed*this->songconfig.rate);
		float t2 = (songconfig.baddis/ dis)*t1;
		Vec2 vGoal =Vec2(vGameArea[rh.pos].x + speedx*t2, vGameArea[rh.pos].y+speedy*t2);//miss时的位置
		nodeQueue[rh.pos].push_back(nodeinfo);
		nodeinfo.index = nodeQueue[rh.pos].size() - 1;

		auto ac = Sequence::create(
			Spawn::create(ScaleTo::create(t1, 1), MoveTo::create(t1, vGameArea[rh.pos]), NULL),
			MoveTo::create(t2, vGoal),
			CCCallFuncN::create([=](Ref *sender)
		{
			nodeinfo.head->setVisible(false);

			Score result = nodeQueue[rh.pos][nodeinfo.index].result;

			if (result == Score::NONE)
				nodeQueue[rh.pos][nodeinfo.index].result = Score::MISS;
			if (result == Score::NONE)
				showScoreEffect(Score::MISS);


		}), DelayTime::create(10), CCCallFuncN::create([=](Ref*sender)
		{
			this->removeChild(nodeinfo.head);
		})
			
			, NULL);
		nodeinfo.head->runAction(ac);
		

		
	}
//	log("Anzer  born-%d end", debug_cnt++);
}
bool MainGame::checkTouch(int pos, const Vec2 &touchLocation)//这里采取的判定区域是在圆内+长方形内
{
	float dis = vGameArea[pos].distance(vBornPoint);
	Rect checkRect = Rect(vGameArea[pos].x - this->songconfig.touchwidth, vGameArea[pos].y - this->songconfig.touchheight, this->songconfig.touchwidth * 2, this->songconfig.touchheight * 2);
	auto point=touchLocation.rotateByAngle(vGameArea[pos], atan((vBornPoint.x - vGameArea[pos].x) / (vBornPoint.y - vGameArea[pos].y)));
	return (checkRect.containsPoint(point) && touchLocation.distance(vGameArea[pos]) < this->songconfig.touchdis);
	
}
void MainGame::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
	for (auto touch : touches)
	{
		auto location = touch->getLocation();
		for (int i = 0; i < 9; i++)
		{
			if (checkTouch(i, location))
			{
				
				while (queueHead[i] < nodeQueue[i].size() && nodeQueue[i][queueHead[i]].result != Score::NONE)queueHead[i]++;
				if (queueHead[i] >= nodeQueue[i].size())
				{
					continue;
				}

				auto tmp = nodeQueue[i][queueHead[i]];
				auto sppos = tmp.head->getPosition();
				float dis = vGameArea[i].distance(sppos);
				if (dis>songconfig.baddis)
				{
					continue;
				}
				Score score = Score::NONE;
				if (dis < songconfig.perfectdis)
				{
					score = (Score::PREJECT);
				}
				else if (dis < songconfig.greatdis)
				{
					score = (Score::GREAT);
				}
				else if (dis < songconfig.gooddis)
				{
					score = (Score::GOOD);
				}
				else
				{
					score = (Score::BAD);
				}
				nodeQueue[i][queueHead[i]].result = score;
				queueHead[i]++;
	
				showPressEffect(i);
				if (tmp.type&RHYTHMTYPE_LONG)
				{
					log("press %d\n", i);
					
					//长条形被按住，停止动作并设置到头像处
					table.insert(std::make_pair(touch, i));
					tmp.head->stopAllActions();
					tmp.head->setPosition(vGameArea[i]);
					tmp.head->setScale(1);
					tmp.noodle->runAction(RepeatForever::create
						(Sequence::create(EaseIn::create(FadeOut::create(1), 2), EaseIn::create(FadeIn::create(1), 2), NULL)));
				}
				else tmp.head->setVisible(false);

				showScoreEffect(score);
			}
		}
	}
}
	


void MainGame::update(float dt)
{


	curTime += dt;
	if ((song.dDuration <= curTime) && curRhythm >=song.lstRhythm.size())//歌曲是否已经到达结束时间了
	{

		this->unscheduleUpdate();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		Stop();
#else
		experimental::AudioEngine::stopAll(); 
#endif
		ResultScene();
		return;
	}
	double speed = this->song.dSpeed;
	while (curRhythm <song.lstRhythm.size() && (song.lstRhythm[curRhythm].beginTime - (speed*songconfig.rate)) <= curTime)
	{
		
		born(song.lstRhythm[curRhythm++]);
		
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
			Score score;


			auto tmp = (nodeQueue[iter->second])[queueHead[iter->second]-1];
			score = tmp.result_tail;
			if (score == Score::NONE)
			{
				auto sppos = tmp.tail->getPosition();
				float dis = vGameArea[iter->second].distance(sppos);
				if (dis<songconfig.perfectdis)
				{
					score = (Score::PREJECT);
				}
				else if (dis < songconfig.greatdis)
				{
					score = (Score::GREAT);
				}
				else if (dis <songconfig.gooddis)
				{
					score = (Score::GOOD);
				}
				else if (dis<songconfig.baddis)
				{
					score = (Score::BAD);
				}
				else score = Score::MISS;
				nodeQueue[iter->second][queueHead[iter->second] - 1].result_tail = score;
			}


			if (nodeQueue[iter->second][queueHead[iter->second] - 1].result_tail != Score::NONE)
			{
				tmp.head->setVisible(false);
				tmp.tail->setVisible(false);
				tmp.noodle->setVisible(false);
				showPressEffect(iter->second);
				showScoreEffect(nodeQueue[iter->second][queueHead[iter->second] - 1].result_tail);
			}
			
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
			Director::getInstance()->replaceScene(SelectSong::createScene());
			return true;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spResult);
	lyResult->addChild(spResult, 1);
//写上得分等数据
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



	auto lbScore = Label::createWithTTF(ttfConfig, "4444444");
	lbScore->setColor(Color3B(255, 102, 153));
	lbScore->setPosition(Vec2(655, 720 - 332));
	lbScore->setAdditionalKerning(20);
	lyResult->addChild(lbScore, 2);

	auto lbHScore = Label::createWithTTF(ttfConfig, "252525252");
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

