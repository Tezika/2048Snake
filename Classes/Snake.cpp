#include"Snake.h"
USING_NS_CC;

using SnakeSpace::Snake;
using SnakeSpace::SnakeNode;
using SnakeSpace::SnakeCtrl;


extern const int TAG_HEAD;
extern const int TAG_BODY;

extern const float TIME_SNAKECHANGE;
extern const int NODE_MAXVAL;

extern const float SPEED_MAX;
extern const float SPEED_MIN;
extern const float RATIO_SPEEDCHANGE;

extern const float SCALE_NODE;

SnakeNode::SnakeNode() :m_pSprite(NULL),m_val(""){}
SnakeNode::~SnakeNode(){}
SnakeNode* SnakeNode::createWithArgs(const std::string& nodeVal, const int tag,const Vec2& pos){
	SnakeNode* node = new SnakeNode();
	if (node&&node->initWithArgs(nodeVal,tag,pos)){
		node->autorelease();
		return node;
	}
	else{
		delete node;
		node = NULL;
		return NULL;
	}
}
bool SnakeNode::initWithArgs(const std::string& nodeVal, const int tag,const Vec2& pos){
	Node::init();
	m_val = nodeVal;
	m_pSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_val+".png"));
	//m_pSprite->setAnchorPoint(Point::ZERO);
	this->setAnchorPoint(Point::ZERO);
	this->setScale(SCALE_NODE);
	this->addChild(m_pSprite);
	this->setTag(tag);
	this->setContentSize(m_pSprite->getContentSize());
	this->setPosition(pos);
	Snake::getSnake()->insert(0,this);



	return true;
}
void SnakeNode::move(const Direction& dir,const Vec2& prePos,const float nodeDis){
	Vec2 movePos;
	switch (dir)
	{
	case Direction::Up:
		movePos = Vec2(prePos.x, prePos.y - nodeDis);
		break;
	case Direction::Down:
		movePos = Vec2(prePos.x, prePos.y + nodeDis);
		break;
	case Direction::Right:
		movePos = Vec2(prePos.x - nodeDis, prePos.y);
		break;
	case Direction::Left:
		movePos = Vec2(prePos.x + nodeDis, prePos.y);
		break;
	default:
		break;
	}
	 this->setPosition(ccpLerp(this->getPosition(), movePos, 0.18f));
	
}
void SnakeNode::move(const Direction& dir, const float speed,const float dt){
	Vec2 speedVec;
	switch (dir){
	case Direction::Up:
		speedVec = Vec2(0, speed);
		break;
	case Direction::Down:
		speedVec = Vec2(0, -speed);
		break;
	case Direction::Right:
		speedVec = Vec2(speed, 0);
		break;
	case Direction::Left:
		speedVec = Vec2(-speed, 0);
		break;
	default:
		break;
	}
	this->runAction(MoveBy::create(dt, speedVec));
}
void SnakeNode::changeNodeVal()
{
	std::stringstream ss_toInt;
	std::stringstream ss_toStr;
	ss_toInt << m_val;
	int curVal;
	ss_toInt>> curVal;
	curVal *= 2;
	if (curVal > NODE_MAXVAL)
	{
		curVal = NODE_MAXVAL;
	}
	UserDefault::getInstance()->setIntegerForKey("CURSCORE", UserDefault::getInstance()->getIntegerForKey("CURSCORE") + curVal);
	ss_toStr << curVal;
	ss_toStr>> m_val;
	auto action_big = ScaleTo::create(TIME_SNAKECHANGE, 2*SCALE_NODE);
	auto action_small = ScaleTo::create(TIME_SNAKECHANGE, SCALE_NODE);
	this->runAction(Sequence::create(CallFunc::create([this](){m_pSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(m_val + ".png")); }), action_big, action_small, NULL));
}
void SnakeNode::removeNode(){
	removeFromParent();
	Snake::getSnake()->eraseObject(this);
}



Snake::Snake(){}
Vector<SnakeNode* >* Snake::m_pSnake = new Vector<SnakeNode* >();
Snake::~Snake(){}
void Snake::removeALLNodes(){
	for (auto it = m_pSnake->begin(); it != m_pSnake->end(); it++)
	{
		(*it)->removeFromParent();
	}
	m_pSnake->clear();
}
SnakeCtrl::SnakeCtrl():m_curDir(Direction::Up),
m_speed(0),
m_pEventListener(NULL)
{}
SnakeCtrl::~SnakeCtrl(){}
bool SnakeCtrl::init(){
	if (!Layer::init()){
		return false;
	}

	m_curDir = Direction::Up;
	m_speed = SPEED_MIN;
	m_nodeDis = 5.0f;

	m_visbleSize = Director::getInstance()->getVisibleSize();
	m_visbleOrginPos = Director::getInstance()->getVisibleOrigin();

	m_newNodeDisVec2[Direction::Up] = Vec2(0, m_nodeDis);
	m_newNodeDisVec2[Direction::Down] = Vec2(0, -m_nodeDis);
	m_newNodeDisVec2[Direction::Left] = Vec2(-m_nodeDis, 0);
	m_newNodeDisVec2[Direction::Right] = Vec2(m_nodeDis, 0);
	return true;
}
void SnakeCtrl::onEnter(){
	Layer::onEnter();

	//add touch listener
	m_pEventListener = EventListenerTouchOneByOne::create();
	m_pEventListener->setEnabled(false);
	m_pEventListener->onTouchBegan = [](Touch* t,Event* e){
		return true;
	};
	m_pEventListener->onTouchEnded = CC_CALLBACK_2(SnakeCtrl::onTouchEndedCallBack, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pEventListener, this);

	//open ths update
	//scheduleUpdate();
}
void SnakeCtrl::onExit(){
	Layer::onExit();
	Snake::removeALLNodes();
}
void SnakeCtrl::onTouchEndedCallBack(Touch* t, Event* e){
	Vec2 delPos = t->getLocation() - t->getStartLocation();
	if (abs(delPos.x) > abs(delPos.y)){
		if (1== Snake::getSnake()->size())
		{
			delPos.x > 0 ? m_curDir = Direction::Right : m_curDir = Direction::Left;
		}
		else
		{
			if (delPos.x > 0 && m_curDir != Direction::Left)
			{
				m_curDir = Direction::Right;
			}
			else if (delPos.x<0 && m_curDir != Direction::Right)
			{
				m_curDir = Direction::Left;
			}
		}
	}
	else{
		if (1 == Snake::getSnake()->size())
		{
			delPos.y > 0 ? m_curDir = Direction::Up : m_curDir = Direction::Down;
		}
		else
		{
			if (delPos.y> 0 && m_curDir != Direction::Down)
			{
				m_curDir = Direction::Up;
			}
			else if (delPos.y<0 && m_curDir != Direction::Up)
			{
				m_curDir = Direction::Down;
			}
		}
	}
}
void SnakeCtrl::update(float dt){
	this->snakeMove(dt);
}

void SnakeCtrl::snakeMove(float dt){
	auto snake = Snake::getSnake();
	snake->front()->move(m_curDir, m_speed>SPEED_MAX?SPEED_MAX:m_speed+RATIO_SPEEDCHANGE*snake->size(), dt);
	for (int index = 1; index < snake->size(); index++){
		snake->at(index)->move(m_curDir, snake->at(index - 1)->getPosition(),m_nodeDis);
	}
}
void SnakeCtrl::setSnakeTags()
{
	Snake::getSnake()->front()->setTag(TAG_HEAD);
	for (int index = 1; index < Snake::getSnake()->size(); index++)
	{
		Snake::getSnake()->at(index)->setTag(TAG_BODY);
	}

}

void SnakeCtrl::combineSnakeNode()
{
	if (Snake::getSnake()->size()>1)
	{
		auto it = ++Snake::getSnake()->begin();
		for (; it != Snake::getSnake()->end();)
		{
			auto sCur = (*it);
			auto sPre = *(it-1);
			if (sCur->getNodeVal() == sPre->getNodeVal())
			{
				sPre->changeNodeVal();
				it = Snake::getSnake()->erase(it);
				sCur->removeNode();
			}
			else
			{
				++it;
			}
		}

	}

}

void SnakeCtrl::addSnakeNode(const std::string& nodeVal )
{
	auto body = SnakeNode::createWithArgs(nodeVal, 
		TAG_HEAD,
		Snake::getSnake()->size()==0?Point(m_visbleOrginPos.x+m_visbleSize.width*0.5,m_visbleOrginPos.y+m_visbleSize.height*0.5):
		Snake::getSnake()->front()->getPosition() + m_newNodeDisVec2[m_curDir]);
	this->addChild(body);
	this->setSnakeTags();
}
void SnakeCtrl::deleteSnakeNode(SnakeNode* s)
{
	std::stringstream ss;
	ss << s->getNodeVal();
	int nodeScore;
	ss >> nodeScore;
	UserDefault::getInstance()->setIntegerForKey("CURSCORE", UserDefault::getInstance()->getIntegerForKey("CURSCORE") - nodeScore);
	s->removeNode();
	this->setSnakeTags();
}

void SnakeCtrl::reverseSnakeNodes()
{
	Snake::getSnake()->reverse();
	deleteSnakeNode(Snake::getSnake()->back());
}
