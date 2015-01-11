#ifndef __CUSTOMDRAWNODE_SCENE_H__
#define __CUSTOMDRAWNODE_SCENE_H__

#include "cocos2d.h"

class CustomDrawNode : public cocos2d::DrawNode//画长条梯形
{
public:
	cocos2d::Sprite *goal;//请勿吐槽命名
	cocos2d::Sprite *start;
	bool pattern;

	virtual bool init();

	void update(float dt);
	double tm;//当前时间
	double atm;//总时间

	CREATE_FUNC(CustomDrawNode);
};
#endif