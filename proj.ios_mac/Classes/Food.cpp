#include"Food.h"

extern const int  NUM_FOODCOW;
extern const int  NUM_FOODROW;

extern const int NUM_INITFOOD_CLASSIC;
extern const int NUM_INITFOOD_CRAZY;

extern const int TAG_NUMFOOD;
extern const int TAG_BOOMFOOD;

extern const float TIME_ADDFOODCOOL_CRAZY ;
extern const float TIME_ADDFOODCOOL_CLASSIC;

extern const float TIME_FOODCHANGE;

extern const float TIME_FOODAPEEARTIME_MAX;
extern const float TIME_FODDAPEEARTIME_MIN;

using SnakeSpace::Food;
using SnakeSpace::Foods;
using SnakeSpace::FoodCtrl;
using SnakeSpace::FoodCreatePos;
using SnakeSpace::FoodCtrl_Classic;
using SnakeSpace::FoodCtrl_Creazy;

USING_NS_CC;

Food::Food():m_pos(NULL){}
Food::~Food(){}
Food* Food::createWithArgs(const std::string& fVal, const int tag, FoodCreatePos* createPos, const float appearTime)
{
	auto pf = new Food();
	if (pf&&pf->initWithArgs(fVal, tag, createPos,appearTime))
	{
		pf->autorelease();
		return pf;
	}
	else{
		delete pf;
		pf = NULL;
		return NULL;
	}
}
bool Food::initWithArgs(const std::string& fVal, const int tag, FoodCreatePos* createPos, const float appearTime)
{
	Node::init();

	m_val = fVal;
	m_pSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f"+m_val+".png"));
	m_pos = createPos;
	m_pos->setIsCreated(true);

	m_appearenctTime = appearTime;
	m_curTime = 0;
	m_shouldDisappear = false;
	m_canEat = false;
	
	this->setAnchorPoint(Point::ZERO);
	this->addChild(m_pSprite);
	this->setTag(tag);
	this->setContentSize(m_pSprite->getContentSize());
	this->setPosition(m_pos->getPos());

	//create action
	this->setScale(0);
	this->runAction(Sequence::create(ScaleTo::create(TIME_FOODCHANGE, 1), 
		CallFunc::create([this](){this->setCanEat(true); }), 
		NULL));


	this->scheduleUpdate();
	Foods::getFoods()->pushBack(this);

	return true;
}
void Food::update(float dt)
{
	m_curTime += dt;
	if (m_curTime >= m_appearenctTime)
	{
		m_shouldDisappear = true;
		m_canEat = false;
		m_curTime = 0;
	}

}
void Food::changeFood(const std::string& val)
{
	m_val = val;
	m_pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f" + m_val + ".png"));
}
void Food::removeFood()
{
	m_pos->setIsCreated(false);
    m_pSprite->removeFromParent();
	removeFromParent();
	Foods::getFoods()->eraseObject(this);
}



FoodCreatePos::FoodCreatePos(){};
FoodCreatePos* FoodCreatePos::createWithArgs(const Vec2& pos)
{
	auto pf = new FoodCreatePos();
	if (pf&&pf->initWithArgs(pos))
	{
		pf->autorelease();
		return pf;
	}
	else{
		delete pf;
		pf = NULL;
		return NULL;
	}
}
bool FoodCreatePos::initWithArgs(const Vec2& pos)
{
	if (!Node::init())
	{
		return false;
	}
	m_isCreate = false;
	m_createPos = pos;
	return true;
}
FoodCreatePos::~FoodCreatePos(){};




Foods::Foods(){};
Vector<Food *>* Foods::m_pFoods = new Vector<Food *>();
Foods::~Foods(){};

void Foods::removeAllFoods()
{
	for (auto it = m_pFoods->begin(); it != m_pFoods->end(); it++)
	{
		(*it)->removeFromParent();
	}
	m_pFoods->clear();
}

FoodCtrl::FoodCtrl():m_pFoodCreatePos(NULL),m_pFoodVals(NULL){};
FoodCtrl::~FoodCtrl(){};

void FoodCtrl::initCreatePos()
{
	Size visbleSize = Director::getInstance()->getVisibleSize();
	Vec2 originPos = Director::getInstance()->getVisibleOrigin();
	for (int i = 1; i < NUM_FOODCOW; i++)
	{
		for (int j = 1; j < NUM_FOODROW; j++)
		{
			auto createPos = FoodCreatePos::createWithArgs(Vec2(originPos.x + i*visbleSize.width / NUM_FOODCOW, originPos.y + j*visbleSize.height / NUM_FOODROW));
			m_pFoodCreatePos->pushBack(createPos);
		}
	}
}

void FoodCtrl::initFoodVals()
{
	m_pFoodVals->push_back(new std::string("2"));
	m_pFoodVals->push_back(new std::string("4"));
	m_pFoodVals->push_back(new std::string("8"));
	m_pFoodVals->push_back(new std::string("16"));
	m_pFoodVals->push_back(new std::string("Boom"));
}

void FoodCtrl::createFood(const std::string& fVal)
{
	bool isCreated = false;
	while (!isCreated)
	{
		auto  newFoodPos = m_pFoodCreatePos->at(CCRANDOM_0_1()*m_pFoodCreatePos->size());
		if (!newFoodPos->getIsCreated())
		{
			isCreated = true;
			auto f = Food::createWithArgs(
				fVal, 
				"Boom"==fVal?TAG_BOOMFOOD:TAG_NUMFOOD, newFoodPos, 
				TIME_FODDAPEEARTIME_MIN+CCRANDOM_0_1()*(TIME_FOODAPEEARTIME_MAX-TIME_FODDAPEEARTIME_MIN)
				);
			this->addChild(f);
		}
	}
}
const std::string& FoodCtrl::getRandFoodVal()
{
	switch (rand()%10)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return *(m_pFoodVals->front());
		break;
	case 4:
	case 5:
	case 6:
		return *(m_pFoodVals->at(1));
		break;
	case 7:
		return *(m_pFoodVals->at(2));
		break;
	case 8:
		return *(m_pFoodVals->at(3));
		break;
	case 9:
		return *(m_pFoodVals->at(4));
	default:
		break;
	}
}
void FoodCtrl::updateFoods()
{
	bool shouldCreate = false;
	for (auto it = Foods::getFoods()->begin(); it != Foods::getFoods()->end();)
	{
		auto f = (*it);
		if (f->getShouldDisappear())
		{
			shouldCreate = true;
			it = Foods::getFoods()->erase(it);
			f->runAction(Sequence::create(ScaleTo::create(TIME_FOODCHANGE,0),CallFunc::create([f](){
				f->removeFood();
			}), NULL));
			break;
		}
		else
		{
			++it;
		}
	}
	if (shouldCreate)
	{
		this->createFood(getRandFoodVal());
	}
	
}

//void FoodCtrl::onExit()
//{
//	Layer::onExit();
//	Foods::removeAllFoods();
//	m_pFoodCreatePos->clear();
//	m_pFoodVals->clear();
//}


/////////////////////////////
FoodCtrl_Classic::FoodCtrl_Classic(){}
FoodCtrl_Classic::~FoodCtrl_Classic(){}

bool FoodCtrl_Classic::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_pFoodCreatePos = new Vector<FoodCreatePos*>();
	m_pFoodVals = new std::vector<std::string *>();
	m_curTime = 0;
	return true;

}
void FoodCtrl_Classic::onEnter()
{
	Layer::onEnter();
	this->initFoodVals();
	this->initCreatePos();
	for (int i = 0; i < NUM_INITFOOD_CLASSIC; i++)
	{
		
	   this->createFood(getRandFoodVal());
		
	}

	//scheduleUpdate();
}

void FoodCtrl_Classic::update(float dt)
{
	m_curTime += dt;
	//add
	if (m_curTime >= TIME_ADDFOODCOOL_CLASSIC)
	{
		//m_isCreating = true;
		this->createFood(getRandFoodVal());
		m_curTime = 0;
	}	
	//update;
	updateFoods();

	
}
void FoodCtrl_Classic::onExit()
{
	Layer::onExit();
	Foods::removeAllFoods();
	m_pFoodCreatePos->clear();
	m_pFoodVals->clear();
}




FoodCtrl_Creazy::FoodCtrl_Creazy(){}
FoodCtrl_Creazy::~FoodCtrl_Creazy(){}

bool FoodCtrl_Creazy::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_pFoodCreatePos = new Vector<FoodCreatePos*>();
	m_pFoodVals = new std::vector<std::string *>();
	m_curTime = 0;
	return true;

}
void FoodCtrl_Creazy::onEnter()
{
	Layer::onEnter();
	this->initFoodVals();
	this->initCreatePos();
	for (int i = 0; i < NUM_INITFOOD_CRAZY; i++)
	{
		this->createFood(getRandFoodVal());

	}

	//scheduleUpdate();
}

void FoodCtrl_Creazy::update(float dt)
{
	m_curTime += dt;
	//add
	if (m_curTime >= TIME_ADDFOODCOOL_CRAZY)
	{
		//m_isCreating = true;
		this->createFood(getRandFoodVal());
		m_curTime = 0;
	}
	//update;
	updateFoods();


}
void FoodCtrl_Creazy::onExit()
{
	Layer::onExit();
	Foods::removeAllFoods();
	m_pFoodCreatePos->clear();
	m_pFoodVals->clear();
}

void FoodCtrl_Creazy::updateFoods()
{
	for (auto it = Foods::getFoods()->begin(); it != Foods::getFoods()->end(); it++)
	{
		auto f = (*it);
		if (f->getShouldDisappear())
		{
			f->changeFood(this->getRandFoodVal());
			if ("Boom"== f->getFoodVal())
			{
				f->setTag(TAG_BOOMFOOD);
			}
			else
			{
				f->setTag(TAG_NUMFOOD);
			}
			f->setCanEat(true);
			f->setShouldDisappear(false);
		}
	}
}