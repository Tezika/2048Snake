#include"GameCtrlLayer.h"
#include"HelloWorldScene.h"
#include"StartScene.h"
#include"SimpleAudioEngine.h"
#include"OverScene.h"

USING_NS_CC;
using namespace CocosDenshion;

#define EFFECT_COLLISION "Collision1.mp3"
#define EFFECT_COLLISIONFALSE "Collision2.wav"
#define FONT_NAME "font.fnt"

extern const int HEALTH_PLAYER;


extern const int TAG_HEAD;
extern const int TAG_BODY;
extern const int TAG_NUMFOOD;
extern const int TAG_BOOMFOOD;
extern const int TAG_BACK;

extern const int GAMELAYERTAGDELTA;

extern const int NODE_MAXVAL;

extern const float CHANCE_FOODCREATE_CLASSIC;
extern const float CHANCE_FOODCREATE_CRAZY;

using SnakeSpace::GameCtrlLayer;
using SnakeSpace::GameCtrlLayer_Classic;
using SnakeSpace::GameCtrlLayer_Creazy;
using SnakeSpace::HelpLayer;
using SnakeSpace::SnakeNode;
using SnakeSpace::Food;

GameCtrlLayer::GameCtrlLayer() :
m_pFoodCtrlLayer(NULL),
m_pSnakeCtrlLayer(NULL),
m_pBgSprite(NULL),
m_pStartLabel(NULL),
m_pEvenTListener(NULL),
m_orginPos(Director::getInstance()->getVisibleOrigin()),
m_visbleSize(Director::getInstance()->getVisibleSize())
{};
GameCtrlLayer::~GameCtrlLayer(){};

void GameCtrlLayer::collision()
{
	bool isCollision = false;
	for (auto it_s = Snake::getSnake()->begin(); it_s != Snake::getSnake()->end(); it_s++)
	{
		for (auto it_f = Foods::getFoods()->begin(); it_f != Foods::getFoods()->end(); it_f++)
		{
			if ((*it_s)->getBoundingBox().intersectsRect((*it_f)->getBoundingBox()) && (*it_f)->getCanEat())
			{
				judgeCollision((*it_s), (*it_f));
				isCollision = true;
				break;
			}

		}
		if (isCollision)
		{
			break;
		}
	}

}

void GameCtrlLayer::judgeCollision(SnakeNode* s, Food* f)
{
	if (TAG_HEAD == s->getTag())
	{
		if (TAG_NUMFOOD == f->getTag())
		{
			if (s->getNodeVal() == f->getFoodVal())
			{

				s->runAction(Sequence::create(CallFunc::create([s](){s->changeNodeVal(); }),
					CallFunc::create(CC_CALLBACK_0(SnakeCtrl::combineSnakeNode, m_pSnakeCtrlLayer))
					, NULL));

				judgeVictory();
			}
			else
			{
				m_pSnakeCtrlLayer->addSnakeNode(f->getFoodVal());
			}

			//rand create new food
			if (CCRANDOM_0_1() >1-CHANCE_FOODCREATE_CLASSIC)
			{
				m_pFoodCtrlLayer->createFood(m_pFoodCtrlLayer->getRandFoodVal());
			}
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISION);
		}
		else if (TAG_BOOMFOOD == f->getTag())
		{
			int subHp;
			if (Snake::getSnake()->size() > 1)
			{
				subHp = 1;
				m_pSnakeCtrlLayer->deleteSnakeNode(s);
			}
			else
			{
				subHp = HEALTH_PLAYER;
			}
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISIONFALSE);
			UserDefault::getInstance()->setIntegerForKey("HEALTH", UserDefault::getInstance()->getIntegerForKey("HEALTH") - subHp);
			this->judgeOver();
		}
		f->removeFood();
	}
	else if (TAG_BODY == s->getTag())
	{
		if (TAG_BOOMFOOD == f->getTag())
		{
			UserDefault::getInstance()->setIntegerForKey("HEALTH", UserDefault::getInstance()->getIntegerForKey("HEALTH") - 1);
			this->judgeOver();
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISIONFALSE);
			m_pSnakeCtrlLayer->deleteSnakeNode(s);
			f->removeFood();
		}
	}
		
	
}

void GameCtrlLayer::judgeVictory()
{
	std::stringstream ss;
	ss << Snake::getSnake()->front()->getNodeVal();
	int headVal;
	ss >> headVal;
	if (headVal >= NODE_MAXVAL)
	{
		//victory
		UserDefault::getInstance()->setIntegerForKey("GAMELAYER", UserDefault::getInstance()->getIntegerForKey("GAMELAYER") + GAMELAYERTAGDELTA);
		UserDefault::getInstance()->setBoolForKey("ISSUCCESSED", true);
		//over
		Director::getInstance()->replaceScene(OverScene::createScene());
	}

}
bool GameCtrlLayer::onTouchBeganCallBack(Touch* t, Event* e)
{
	if (t != NULL)
	{
		if (NULL != m_pEvenTListener)
		{
			m_pEvenTListener->setEnabled(false);
		}
		this->getSnakeCtrlLayer()->scheduleUpdate();
		if (NULL != this->getSnakeCtrlLayer()->getEventListener())
		{
			this->getSnakeCtrlLayer()->getEventListener()->setEnabled(true);
		}
		this->getFoodCtrlLayer()->scheduleUpdate();
		m_pStartLabel->removeFromParent();
		
		
	}
	return false;
}
void GameCtrlLayer::judgeOver()
{
	if (0 >= UserDefault::getInstance()->getIntegerForKey("HEALTH"))
	{
		UserDefault::getInstance()->setIntegerForKey("GAMELAYER", UserDefault::getInstance()->getIntegerForKey("GAMELAYER") + GAMELAYERTAGDELTA);
		UserDefault::getInstance()->setBoolForKey("ISSUCCESSED", false);
		//over
		Director::getInstance()->replaceScene(OverScene::createScene());
	}
	
}
bool GameCtrlLayer::checkUpBouding()
{
	if (Snake::getSnake()->front()->getPositionY() >= m_visbleSize.height)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool GameCtrlLayer::checkDownBounding()
{
	if (Snake::getSnake()->front()->getPositionY() <= m_orginPos.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool GameCtrlLayer::checkRightBounding()
{
	if (Snake::getSnake()->front()->getPositionX() >= m_visbleSize.width)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool GameCtrlLayer::checkLeftBounding()
{
	if (Snake::getSnake()->front()->getPositionX() <= m_orginPos.x)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void GameCtrlLayer::checkBouding()
{
	auto snake = Snake::getSnake();
	if (checkUpBouding())
	{
		m_pSnakeCtrlLayer->setCurDir(Direction::Down);
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISIONFALSE);
		if (snake->size() != 1)
		{
			
			m_pSnakeCtrlLayer->reverseSnakeNodes();
			
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("HEALTH", UserDefault::getInstance()->getIntegerForKey("HEALTH") - HEALTH_PLAYER);
			this->judgeOver();
		}
	}
	else if (checkDownBounding())
	{
		m_pSnakeCtrlLayer->setCurDir(Direction::Up);
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISIONFALSE);
		if (snake->size() != 1)
		{
			
			m_pSnakeCtrlLayer->reverseSnakeNodes();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("HEALTH", UserDefault::getInstance()->getIntegerForKey("HEALTH") - HEALTH_PLAYER);
			this->judgeOver();
		}

	}
	else if (checkLeftBounding())
	{
		m_pSnakeCtrlLayer->setCurDir(Direction::Right);
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISIONFALSE);
		if (snake->size() != 1)
		{
			
			m_pSnakeCtrlLayer->reverseSnakeNodes();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("HEALTH", UserDefault::getInstance()->getIntegerForKey("HEALTH") - HEALTH_PLAYER);
			this->judgeOver();
		}

	}
	else if (checkRightBounding())
	{
		m_pSnakeCtrlLayer->setCurDir(Direction::Left);
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISIONFALSE);
		if (snake->size() != 1)
		{
			
			m_pSnakeCtrlLayer->reverseSnakeNodes();
		}
		else
		{
			UserDefault::getInstance()->setIntegerForKey("HEALTH", UserDefault::getInstance()->getIntegerForKey("HEALTH") - HEALTH_PLAYER);
			this->judgeOver();
		}
	}

}



GameCtrlLayer_Classic::GameCtrlLayer_Classic(){};
GameCtrlLayer_Classic::~GameCtrlLayer_Classic(){};
bool GameCtrlLayer_Classic::init()
{
	if (!Layer::init())
	{
		return false;
	}
	

	this->setSnakeCtrlLayer(SnakeCtrl::create());
	this->addChild(this->getSnakeCtrlLayer());
	
	this->setFoodCtrlLayer(FoodCtrl_Classic::create());
	this->addChild(this->getFoodCtrlLayer());

	m_pScoreLable = LabelBMFont::create("", FONT_NAME,40);
	m_pScoreLable->setAnchorPoint(Point::ZERO);
	m_pScoreLable->setPosition(m_orginPos.x + m_visbleSize.width*0.3, m_orginPos.y+ m_visbleSize.height*0.92);
	this->addChild(m_pScoreLable);

	m_pHealthLable = LabelBMFont::create("", FONT_NAME, 40);
	m_pHealthLable->setAnchorPoint(Point::ZERO);
	m_pHealthLable->setPosition(m_orginPos.x + m_visbleSize.width*0.6, m_orginPos.x + m_visbleSize.height*0.92);
	this->addChild(m_pHealthLable);


	m_pStartLabel = LabelBMFont::create("Tap to start!", FONT_NAME, 40);
	m_pStartLabel->setPosition(m_orginPos.x + m_visbleSize.width*0.5, m_orginPos.y + m_visbleSize.height*0.6);
	this->addChild(m_pStartLabel);

	m_pBgSprite = Sprite::create();
	m_pBgSprite->setTextureRect(Rect(0, 0, 2*m_visbleSize.width, 2*m_visbleSize.height));
	m_pBgSprite->setColor(Color3B(190, 173, 146));
	m_pBgSprite->setPosition(Point::ZERO);
	this->addChild(m_pBgSprite,-1);

	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_COLLISION);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_COLLISIONFALSE);

	return true;
}
void GameCtrlLayer_Classic::onEnter()
{
	Layer::onEnter();
	scheduleUpdate();
	UserDefault::getInstance()->setIntegerForKey("HEALTH", HEALTH_PLAYER);
	UserDefault::getInstance()->setIntegerForKey("CURSCORE", 0);

	this->getSnakeCtrlLayer()->addSnakeNode("2");


     m_pEvenTListener= EventListenerTouchOneByOne::create();
	 m_pEvenTListener->onTouchBegan = CC_CALLBACK_2(GameCtrlLayer::onTouchBeganCallBack, this);
	 m_pEvenTListener->setSwallowTouches(false);
	 Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEvenTListener, this);
}
void GameCtrlLayer_Classic::update(float dt)
{

	this->collision();
	this->checkBouding();

	char str[20];
	sprintf(str, "Score:%d", UserDefault::getInstance()->getIntegerForKey("CURSCORE"));
	m_pScoreLable->setString(str);
	sprintf(str, "Health:%d", UserDefault::getInstance()->getIntegerForKey("HEALTH"));
	m_pHealthLable->setString(str);

}
void GameCtrlLayer_Classic::onExit()
{
	if (UserDefault::getInstance()->getIntegerForKey("CURSCORE") > UserDefault::getInstance()->getIntegerForKey("BESTSCORE_CLASSIC"))
	{
		UserDefault::getInstance()->setIntegerForKey("BESTSCORE_CLASSIC", UserDefault::getInstance()->getIntegerForKey("CURSCORE"));
	}
	Layer::onExit();

}


///////////////
GameCtrlLayer_Creazy::GameCtrlLayer_Creazy(){}
GameCtrlLayer_Creazy::~GameCtrlLayer_Creazy(){}

bool GameCtrlLayer_Creazy::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setSnakeCtrlLayer(SnakeCtrl::create());
	this->addChild(this->getSnakeCtrlLayer());

	this->setFoodCtrlLayer(FoodCtrl_Creazy::create());
	this->addChild(this->getFoodCtrlLayer());

	m_pScoreLable = LabelBMFont::create("", FONT_NAME, 40);
	m_pScoreLable->setAnchorPoint(Point::ZERO);
	m_pScoreLable->setPosition(m_orginPos.x + m_visbleSize.width*0.3, m_orginPos.y + m_visbleSize.height*0.92);
	this->addChild(m_pScoreLable);

	m_pHealthLable = LabelBMFont::create("", FONT_NAME, 40);
	m_pHealthLable->setAnchorPoint(Point::ZERO);
	m_pHealthLable->setPosition(m_orginPos.x + m_visbleSize.width*0.6, m_orginPos.x + m_visbleSize.height*0.92);
	this->addChild(m_pHealthLable);


	m_pStartLabel = LabelBMFont::create("Tap to start!", FONT_NAME, 40);
	m_pStartLabel->setPosition(m_orginPos.x + m_visbleSize.width*0.5, m_orginPos.y + m_visbleSize.height*0.6);
	this->addChild(m_pStartLabel);

	m_pBgSprite = Sprite::create();
	m_pBgSprite->setTextureRect(Rect(0, 0, 2 * m_visbleSize.width, 2 * m_visbleSize.height));
	m_pBgSprite->setColor(Color3B(190, 173, 146));
	m_pBgSprite->setPosition(Point::ZERO);
	this->addChild(m_pBgSprite, -1);

	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_COLLISION);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_COLLISIONFALSE);

	return true;
}
void GameCtrlLayer_Creazy::onEnter()
{
	Layer::onEnter();
	scheduleUpdate();
	UserDefault::getInstance()->setIntegerForKey("HEALTH", HEALTH_PLAYER);
	UserDefault::getInstance()->setIntegerForKey("CURSCORE", 0);

	this->getSnakeCtrlLayer()->addSnakeNode("2");


	m_pEvenTListener = EventListenerTouchOneByOne::create();
	m_pEvenTListener->onTouchBegan = CC_CALLBACK_2(GameCtrlLayer::onTouchBeganCallBack, this);
	m_pEvenTListener->setSwallowTouches(false);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEvenTListener, this);

}
void GameCtrlLayer_Creazy::update(float dt)
{
	this->collision();
	this->checkBouding();

	char str[20];
	sprintf(str, "Score:%d", UserDefault::getInstance()->getIntegerForKey("CURSCORE"));
	m_pScoreLable->setString(str);
	sprintf(str, "Health:%d", UserDefault::getInstance()->getIntegerForKey("HEALTH"));
	m_pHealthLable->setString(str);

}
void GameCtrlLayer_Creazy::judgeCollision(SnakeNode* s, Food* f)
{
	if (TAG_HEAD == s->getTag())
	{
		if (TAG_NUMFOOD == f->getTag())
		{
			if (s->getNodeVal() == f->getFoodVal())
			{

				s->runAction(Sequence::create(CallFunc::create([s](){s->changeNodeVal(); }),
					CallFunc::create(CC_CALLBACK_0(SnakeCtrl::combineSnakeNode, this->getSnakeCtrlLayer()))
					, NULL));
				judgeVictory();
			}
			else
			{
				this->getSnakeCtrlLayer()->addSnakeNode(f->getFoodVal());
			}
			//rand create new food
			if (CCRANDOM_0_1() >1 - CHANCE_FOODCREATE_CRAZY)
			{
				this->getFoodCtrlLayer()->createFood(this->getFoodCtrlLayer()->getRandFoodVal());
			}
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISION);
		}
		else if (TAG_BOOMFOOD == f->getTag())
		{
			int subHp;
			if (Snake::getSnake()->size() > 1)
			{
				subHp = 1;
				this->getSnakeCtrlLayer()->deleteSnakeNode(s);
			}
			else
			{
				subHp = HEALTH_PLAYER;
			}
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISIONFALSE);
			UserDefault::getInstance()->setIntegerForKey("HEALTH", UserDefault::getInstance()->getIntegerForKey("HEALTH") - subHp);
			this->judgeOver();
		}
		f->removeFood();
	}
	else if (TAG_BODY == s->getTag())
	{
		if (TAG_BOOMFOOD == f->getTag())
		{
			UserDefault::getInstance()->setIntegerForKey("HEALTH", UserDefault::getInstance()->getIntegerForKey("HEALTH") - 1);
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_COLLISIONFALSE);
			this->judgeOver();
			this->getSnakeCtrlLayer()->deleteSnakeNode(s);
			f->removeFood();
		}
	}


}

void GameCtrlLayer_Creazy::onExit()
{
	
	if (UserDefault::getInstance()->getIntegerForKey("CURSCORE") > UserDefault::getInstance()->getIntegerForKey("BESTSCORE_CRAZY"))
	{
		UserDefault::getInstance()->setIntegerForKey("BESTSCORE_CRAZY", UserDefault::getInstance()->getIntegerForKey("CURSCORE"));
	}
	Layer::onExit();

}


HelpLayer::HelpLayer(){}
HelpLayer::~HelpLayer(){}
bool HelpLayer::init()
{

	if (!Layer::init())
	{
		return false;
	}
	auto orgin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bg = Sprite::create("help.png");
	bg->setPosition(orgin.x + visibleSize.width / 2, orgin.y + visibleSize.height / 2);
	this->addChild(bg);

	m_pBack = Label::createWithBMFont(FONT_NAME, "Back");
	m_pBack->setPosition(orgin.x + visibleSize.width*0.8, orgin.y + visibleSize.height*0.1);
	m_pBack->setTag(TAG_BACK);
	this->addChild(m_pBack);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* t,Event* e){
		if (this->getChildByTag(TAG_BACK)->getBoundingBox().containsPoint(t->getLocation()))
		{

			Director::getInstance()->replaceScene(TransitionFade::create(0.8f, StartScene::createScene()));
		}
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}