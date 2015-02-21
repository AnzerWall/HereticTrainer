#ifndef __SELECTSONG_SCENE_H__
#define __SELECTSONG_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"DataManager.h"
class SelectSong : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	void createSprite(int);
	void removeSprite();
	cocos2d::Sprite* spTitle;
	cocos2d::Label* lbName;
	cocos2d::ui::CheckBox *cbRandom;
	int curPos = 0;
	std::vector<SongInfo> songlist;
	CREATE_FUNC(SelectSong);
};

#endif 