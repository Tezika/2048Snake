#include"StartScene.h"
#include"HelloWorldScene.h"

USING_NS_CC;

extern const int TAG_CLASSIC;
extern const int TAG_CRAZY;
extern const int TAG_HELP;
extern const int TAG_EXIT;



Scene* StartScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
bool StartScene::init()
{
	if (!Layer::init())
	{
		return false;
   }
	auto visbleSize = Director::getInstance()->getVisibleSize();
	m_buttonSize = Size(visbleSize.width / 2, visbleSize.height / 2);
	this->setStartButton(TAG_CLASSIC, Color3B(238, 228, 218), Color3B::BLACK,"Classic", Vec2(0, visbleSize.height / 2 ));
	this->setStartButton(TAG_CRAZY, Color3B(250, 248, 249), Color3B::BLACK, "Crazy", Vec2(visbleSize.width / 2 , visbleSize.height / 2 ));
	this->setStartButton(TAG_HELP, Color3B(237, 224, 200), Color3B::BLACK,"Help", Vec2(0, 0));
	this->setStartButton(TAG_EXIT, Color3B(242, 177, 121), Color3B::WHITE,"Exit", Vec2(visbleSize.width / 2, 0));

    //how to implement the touch?
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StartScene::onTouchBeganCallBack, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	return true;
}
bool StartScene::onTouchBeganCallBack(Touch* t, Event* e)
{
	if (this->getChildByTag(TAG_CLASSIC)->getBoundingBox().containsPoint(t->getLocation()))
	{
		UserDefault::getInstance()->setIntegerForKey("GAMELAYER", TAG_CLASSIC);
		Director::getInstance()->replaceScene(TransitionFadeUp::create(0.8f,HelloWorld::createScene()));
	}
	else if (this->getChildByTag(TAG_CRAZY)->getBoundingBox().containsPoint(t->getLocation()))
	{
		UserDefault::getInstance()->setIntegerForKey("GAMELAYER", TAG_CRAZY);
		Director::getInstance()->replaceScene(TransitionFadeUp::create(0.8f, HelloWorld::createScene()));
	}
	else if (this->getChildByTag(TAG_HELP)->getBoundingBox().containsPoint(t->getLocation()))
	{
		UserDefault::getInstance()->setIntegerForKey("GAMELAYER", TAG_HELP);
		Director::getInstance()->replaceScene(TransitionFadeUp::create(0.8f, HelloWorld::createScene()));
	}
	else if (this->getChildByTag(TAG_EXIT)->getBoundingBox().containsPoint(t->getLocation()))
	{
        Director::getInstance()->end();
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
        #endif
	}
	return false;
}
void StartScene::setStartButton(const int& tag, const Color3B& color,const Color3B& tcolor, const std::string& text, const Vec2& Pos)
{
	auto b = Sprite::create();
    b->setTag(tag);
	b->setColor(color);
	b->setContentSize(m_buttonSize);
	b->setAnchorPoint(Point::ZERO);
	b->setTextureRect(Rect(0, 0, m_buttonSize.width, m_buttonSize.height));
	b->setPosition(Pos);
	auto str = LabelBMFont::create(text, "font.fnt",100);
	b->addChild(str);
	str->setPosition(b->getContentSize().width / 2, b->getContentSize().height / 2);
	str->setColor(tcolor);
	this->addChild(b);

}
