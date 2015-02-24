#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__
#include<queue>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"CustomDrawNode.h"
#include"DataManager.h"
#include<unordered_map>
#include "MusicPlayer.h"
using cocos2d::Vec2;
enum class Score{ PREJECT = 1, GREAT,GOOD,BAD,MISS,NONE };



struct NodeInfo
{
	cocos2d::Sprite* head;
	cocos2d::Sprite* tail;
	CustomDrawNode* noodle;
	int type;
	int index;
	Score result;
	Score result_tail;
	NodeInfo()
	{	
		head = NULL;
		tail=NULL;
		noodle = NULL;
		type=0;
		index=0;
		result = Score::NONE;
		result_tail = Score::NONE;
	}
	
};
class MainGame : public cocos2d::Layer
{
public:
	//保存一些对象的指针方便操作
	cocos2d::EventListenerTouchAllAtOnce* listener;
	cocos2d::Layer *touchLayer;
	cocos2d::Sprite *spPerfect;
	cocos2d::Sprite *spGreat;
	cocos2d::Sprite *spGood;
	cocos2d::Sprite *spBad;
	cocos2d::Sprite *spMiss;
	cocos2d::Label* lbCombo;
	cocos2d::Label*  lbComboCnt;
	//int idAudio;//背景音乐的id
	cocos2d::ui::Button* btStop;

	int curRhythm;
	double curTime;
	//记录
	int maxCombo;
	int curCombo;
	int cntPerfect;
	int cntGreat;
	int cntGood;
	int cntBad;
	int cntMiss;


	
	Song song;//当前的歌曲数据
	SongInfo songinfo;
	SongConfig songconfig;


	std::vector<NodeInfo> nodeQueue[9];
	int queueHead[9];
	std::unordered_map<cocos2d::Touch*, int > table;//保存某次触摸是属于哪一个道的圆环的
	MainGame()
	{
		curRhythm = 0;
		curTime = 0;
		maxCombo = 0;
		curCombo = 0;
		cntPerfect = 0;
		cntGreat = 0;
		cntGood = 0;
		cntBad = 0;
		cntMiss = 0;
	}
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(const SongInfo &songinfo,const Song &song, const SongConfig &songfig);
	virtual bool init(const SongInfo& songinfo, const Song &song, const SongConfig &songfig);

	static MainGame* create(const SongInfo &songinfo, const Song &song, const SongConfig &songfig)
	{  
		MainGame *pRet = new MainGame(); 
		if (pRet && pRet->init(songinfo,song,songfig))
		{ 
			pRet->autorelease(); 
			return pRet; 
		} 
		else 
		{ 
			delete pRet; 
			pRet = NULL; 
			return NULL; 
		} 
	}
	void Init_Spr_Score_cb(); //创造combo和分数评价的spr
	void Init_TouchLayer();//创建触摸层
	void Init_Background();//背景图绘制
	void born(const Rhythm &rh);//c产生圆环
	//触摸事件
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	//帧更新
	 void update(float dt);
	 //显示点击动画
	 void showPressEffect(int pos);
	 //显示评分动画
	 void showScoreEffect(Score score);
	 //暂停界面
	 void  StopSence();
	 //结果界面
	 void  ResultScene();
	 //测试是否触摸了某一道
	 bool checkTouch(int pos, const Vec2 &touchLocation);

};

#endif 
