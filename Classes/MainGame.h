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
	//����һЩ�����ָ�뷽�����
	cocos2d::EventListenerTouchAllAtOnce* listener;
	cocos2d::Layer *touchLayer;
	cocos2d::Sprite *spPerfect;
	cocos2d::Sprite *spGreat;
	cocos2d::Sprite *spGood;
	cocos2d::Sprite *spBad;
	cocos2d::Sprite *spMiss;
	cocos2d::Label* lbCombo;
	cocos2d::Label*  lbComboCnt;
	//int idAudio;//�������ֵ�id
	cocos2d::ui::Button* btStop;

	int curRhythm = 0;
	double curTime = 0;
	//��¼
	int maxCombo = 0;
	int curCombo = 0;
	int cntPerfect = 0;
	int cntGreat = 0;
	int cntGood = 0;
	int cntBad = 0;
	int cntMiss = 0;

	std::string songpath;//��������·��

	bool songbegin = false;//�Ƿ��Ѿ���ʼ��������
	bool endGame = false;//�Ƿ������Ϸ
	Song song;//��ǰ�ĸ�������
	double rate=1;//���ʣ�δʹ��

	//�ж���׼
	double missdis=160;
	double perfectdis=16;//15
	double greatdis=42;//35
	double gooddis=85;//65
	double baddis=160;
	double touchdis =140;
	double touchwid = 80;//�����ж����εĿ�
	double touchhei = 145;//half



	std::queue<GameObject> q[9];//����ÿһ�������Բ��

	std::unordered_map<cocos2d::Touch*, int > table;//����ĳ�δ�����������һ������Բ����

	void born(const Rhythm &rh);//c����Բ��
	//�����¼�
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	//֡����
	 void update(float dt);
	 //��ʾ�������
	 void showPressEffect(int pos);
	 //��ʾ���ֶ���
	 void showScoreEffect(Score score);
	 //��ͣ����
	 void  StopSence();
	 //�������
	 void  ResultScene();
	 //�����Ƿ�����ĳһ��
	 bool checkTouch(int pos, const Vec2 &touchLocation);
	//���㼸�Σ����ˣ�����ֵ>0��ʾ�߶�BO��AO����ࣨ��ʱ�뷽�򣩣�<0��ʾ���Ҳ࣬0ͬ�ࣨ����ͬ��Ҳ���ܷ��򣩣�δʹ��
	inline float cross(Vec2 vO, Vec2 vA, Vec2 vB) 
	{
		return (vA.x - vO.x)*(vB.y - vO.y) - (vA.y - vO.y)*(vB.x - vO.x);
	}
};

#endif 
