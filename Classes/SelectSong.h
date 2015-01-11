#ifndef __SELECTSONG_SCENE_H__
#define __SELECTSONG_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SelectSong : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	void createSprite(int);
	void removeSprite();
	cocos2d::Sprite* spTitle;
	cocos2d::Label* lbName;

	int curPos = 0;
	std::vector<std::string> filelist;
	CREATE_FUNC(SelectSong);
};

#endif 