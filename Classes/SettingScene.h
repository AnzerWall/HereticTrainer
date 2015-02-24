#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
class SettingScene : public cocos2d::Layer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();
	cocos2d::ui::Slider *slPrefect;
	cocos2d::ui::Slider *slGreat;
	cocos2d::ui::Slider *slGood;
	cocos2d::ui::Slider *slBad;
	cocos2d::Label	* lbPerfect;
	cocos2d::Label	* lbGreat;
	cocos2d::Label	* lbGood;
	cocos2d::Label	* lbBad;
	// implement the "static node()" method manually
	CREATE_FUNC(SettingScene);
	void setSilder();
	void sliderEventPerfect(Ref *pSender, cocos2d::ui::Slider::EventType type);
	void sliderEventGreat(Ref *pSender, cocos2d::ui::Slider::EventType type);
	void sliderEventGood(Ref *pSender, cocos2d::ui::Slider::EventType type);
	void sliderEventBad(Ref *pSender, cocos2d::ui::Slider::EventType type);
};

#endif // __HELLOWORLD_SCENE_H__
