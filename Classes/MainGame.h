#ifndef __MAINGAME_SCENE_H__
#define __MAINGAME_SCENE_H__
#include<queue>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"CustomDrawNode.h"
#include"DataManager.h"
#include<unordered_map>
using cocos2d::Vec2;
enum class Score{ PREJECT = 1, GREAT,GOOD,BAD,MISS };


struct  GameObject 
{
	cocos2d::Sprite* start;
	cocos2d::Sprite* goal;
	CustomDrawNode* node;
	int type;
};
class MainGame : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(const Song &song, std::string songpath, double speedrate);
	virtual bool init();
	CREATE_FUNC(MainGame);
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

	int curRhythm = 0;
	double curTime = 0;
	//记录
	int maxCombo = 0;
	int curCombo = 0;
	int cntPerfect = 0;
	int cntGreat = 0;
	int cntGood = 0;
	int cntBad = 0;
	int cntMiss = 0;

	std::string songpath;//背景音乐路径

	bool songbegin = false;//是否已经开始播放音乐
	bool endGame = false;//是否结束游戏
	Song song;//当前的歌曲数据
	double rate=1;//速率，未使用

	//判定基准
	double missdis=160;
	double perfectdis=16;//15
	double greatdis=42;//35
	double gooddis=85;//65
	double baddis=160;
	double touchdis =140;
	double touchwid = 80;//触摸判定矩形的宽
	double touchhei = 145;//half



	std::queue<GameObject> q[9];//保存每一道最近的圆环

	std::unordered_map<cocos2d::Touch*, int > table;//保存某次触摸是属于哪一个道的圆环的

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
	//计算几何，求叉乘，返回值>0表示线段BO在AO的左侧（逆时针方向），<0表示在右侧，0同侧（可能同向也可能反向），未使用
	inline float cross(Vec2 vO, Vec2 vA, Vec2 vB) 
	{
		return (vA.x - vO.x)*(vB.y - vO.y) - (vA.y - vO.y)*(vB.x - vO.x);
	}
};

#endif 
