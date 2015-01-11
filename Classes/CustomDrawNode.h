#ifndef __CUSTOMDRAWNODE_SCENE_H__
#define __CUSTOMDRAWNODE_SCENE_H__

#include "cocos2d.h"

class CustomDrawNode : public cocos2d::DrawNode//����������
{
public:
	cocos2d::Sprite *goal;//�����²�����
	cocos2d::Sprite *start;
	bool pattern;

	virtual bool init();

	void update(float dt);
	double tm;//��ǰʱ��
	double atm;//��ʱ��

	CREATE_FUNC(CustomDrawNode);
};
#endif