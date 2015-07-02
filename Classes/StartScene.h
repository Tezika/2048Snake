#pragma once
#include"cocos2d.h"
USING_NS_CC;
class StartScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);
public:
	bool onTouchBeganCallBack(Touch* t, Event* e);
private:
	Size m_buttonSize;
	void setStartButton(const int& tag, const Color3B& color, const Color3B& tColor,const std::string& text, const Vec2& Pos);

};