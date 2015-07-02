#include"OverLayer.h"
#include"HelloWorldScene.h"
#include"StartScene.h"

#define FONTNAME "font.fnt"
using SnakeSpace::OverLayerBase;

extern const int TAG_CLASSIC;
extern const int TAG_CRAZY;

extern const int TAG_RESTART;
extern const int TAG_BACK;
extern const int GAMELAYERTAGDELTA;

OverLayerBase::OverLayerBase():
m_pBack(Label::createWithBMFont(FONTNAME,"Back" )),
m_pReStart(Label::createWithBMFont(FONTNAME,"ReStart")),
m_visbleSize(Director::getInstance()->getVisibleSize()),
m_visbleOrgin(Director::getInstance()->getVisibleOrigin()),
m_pBestScore(NULL),
m_pCureScore(NULL),
m_pModeLabel(NULL),
m_pSuccessOrFail(NULL)
{
	m_pBack->setTag(TAG_BACK);
	m_pReStart->setTag(TAG_RESTART);
}
OverLayerBase::~OverLayerBase(){}

bool  OverLayerBase::init()
{

	if (!Layer::init())
	{
		return false;
	}
    auto bg = Sprite::create();
    bg->setTextureRect(Rect(0, 0, m_visbleSize.width, m_visbleSize.height));
    bg->setPosition(m_visbleOrgin.x + m_visbleSize.width*0.5, m_visbleOrgin.y + m_visbleSize.height*0.5);
    bg->setColor(Color3B(242, 177, 121));
    this->addChild(bg);
	return true;
}
void OverLayerBase::onEnter()
{
    Layer::onEnter();
    char str[20];
    sprintf(str, "Score:%d", UserDefault::getInstance()->getIntegerForKey("CURSCORE"));
    m_pCureScore = LabelBMFont::create(str, FONTNAME);
    if (UserDefault::getInstance()->getBoolForKey("ISSUCCESSED"))
    {
        m_pSuccessOrFail = LabelBMFont::create("Congratulation!", FONTNAME);
    }
    
    if (TAG_CLASSIC == UserDefault::getInstance()->getIntegerForKey("GAMELAYER") - GAMELAYERTAGDELTA)
    {
        m_pModeLabel = LabelBMFont::create("Classic", FONTNAME);
        sprintf(str, "Best:%d", UserDefault::getInstance()->getIntegerForKey("BESTSCORE_CLASSIC"));
        m_pBestScore = LabelBMFont::create(str, FONTNAME);
    }
    else
    {
        m_pModeLabel = LabelBMFont::create("Crazy", FONTNAME);
        sprintf(str, "Best:%d", UserDefault::getInstance()->getIntegerForKey("BESTSCORE_CRAZY"));
        m_pBestScore = LabelBMFont::create(str, FONTNAME);
    }
    this->setLabelPos();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(OverLayerBase::onTouchCallBack, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
void OverLayerBase::onExit()
{
    Layer::onExit();
}


void OverLayerBase::setLabelPos()
{
	m_pModeLabel->setPosition(m_visbleOrgin.x + m_visbleSize.width*0.5, m_visbleOrgin.y + m_visbleSize.height*0.9);
	m_pModeLabel->setScale(2);
	this->addChild(m_pModeLabel);

	m_pCureScore->setPosition(m_visbleOrgin.x + m_visbleSize.width*0.5, m_visbleOrgin.y + m_visbleSize.height*0.55);
	this->addChild(m_pCureScore);

	m_pBestScore->setPosition(m_visbleOrgin.x + m_visbleSize.width*0.5, m_visbleOrgin.y + m_visbleSize.height*0.4);
	this->addChild(m_pBestScore);

	m_pBack->setPosition(m_visbleOrgin.x + m_visbleSize.width*0.8, m_visbleOrgin.y + m_visbleSize.height*0.1);
	this->addChild(m_pBack);

	m_pReStart->setPosition(m_visbleOrgin.x + m_visbleSize.width*0.2, m_visbleOrgin.y + m_visbleSize.height*0.1);
	this->addChild(m_pReStart);
}

bool OverLayerBase::onTouchCallBack(Touch *t, Event* e)
{
	if (this->getChildByTag(TAG_RESTART)->getBoundingBox().containsPoint(t->getLocation()))
	{
		UserDefault::getInstance()->setIntegerForKey("GAMELAYER", UserDefault::getInstance()->getIntegerForKey("GAMELAYER") - GAMELAYERTAGDELTA);
		Director::getInstance()->replaceScene(TransitionFade::create(0.8f, HelloWorld::createScene()));
	}
	else if (this->getChildByTag(TAG_BACK)->getBoundingBox().containsPoint(t->getLocation()))
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.8f, StartScene::createScene()));
	}
	return false;
}
